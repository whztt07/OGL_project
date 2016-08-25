#include <cfloat>
#include <math.h>
#include <GL/glew.h>
#include <string>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif
#include <sys/types.h>

#include "ogldev_engine_common.h"
#include "ogldev_app.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_backend.h"
#include "ogldev_camera.h"
#include "ogldev_lights_common.h"
#include "ogldev_shadow_map_fbo.h"
#include "ogldev_atb.h"
#include "ogldev_io_buffer.h"
#include "ssao_technique.h"
#include "geom_pass_tech.h"
#include "blur_tech.h"
#include "mesh.h"
#include "lighting_technique.h"

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 1024

Quaternion g_Rotation = Quaternion(0.707f, 0.0f, 0.0f, 0.707f);

int gGLMajorVersion = 0;

class Main : public ICallbacks, public OgldevApp
{
public:

	Main()
	{
		m_pGameCamera = NULL;

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 5000.0f;

		m_pipeline.SetPerspectiveProj(m_persProjInfo);

		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.3f;
		m_directionalLight.DiffuseIntensity = 1.0f;
		m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

		m_shaderType = 0;

		glGetIntegerv(GL_MAJOR_VERSION, &gGLMajorVersion);
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pGameCamera);
	}

	bool Init()
	{
		if (!m_atb.Init()) {
			return false;
		}

		Vector3f Pos(0.0f, 24.0f, -38.0f);
		Vector3f Target(0.0f, -0.5f, 1.0f);

		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		if (!m_geomPassTech.Init()) {
			OGLDEV_ERROR("Error initializing the geometry pass technique\n");
			return false;
		}

		if (!m_SSAOTech.Init()) {
			OGLDEV_ERROR("Error initializing the SSAO technique\n");
			return false;
		}

		m_SSAOTech.Enable();
		m_SSAOTech.SetSampleRadius(1.5f);
		Matrix4f PersProjTrans;
		PersProjTrans.InitPersProjTransform(m_persProjInfo);
		m_SSAOTech.SetProjMatrix(PersProjTrans);
		float AspectRatio = m_persProjInfo.Width / m_persProjInfo.Height;
		m_SSAOTech.SetAspectRatio(AspectRatio);
		float TanHalfFOV = tanf(ToRadian(m_persProjInfo.FOV / 2.0f));
		m_SSAOTech.SetTanHalfFOV(TanHalfFOV);

		if (!m_lightingTech.Init()) {
			OGLDEV_ERROR("Error initializing the lighting technique\n");
			return false;
		}

		m_lightingTech.Enable();
		m_lightingTech.SetDirectionalLight(m_directionalLight);
		m_lightingTech.SetScreenSize(WINDOW_WIDTH, WINDOW_HEIGHT);
		m_lightingTech.SetShaderType(0);

		if (!m_blurTech.Init()) {
			OGLDEV_ERROR("Error initializing the blur technique\n");
			return false;
		}

		if (!m_mesh.LoadMesh("thirdparty/content/jeep.obj")) {
			return false;
		}

		m_mesh.GetOrientation().m_scale = Vector3f(0.05f);
		m_mesh.GetOrientation().m_pos = Vector3f(0.0f, 0.0f, 0.0f);
		m_mesh.GetOrientation().m_rotation = Vector3f(0.0f, 180.0f, 0.0f);

		if (!m_quad.LoadMesh("thirdparty/content/quad.obj")) {
			return false;
		}

		if (!m_depthBuffer.Init(WINDOW_WIDTH, WINDOW_HEIGHT, true, GL_NONE)) {
			return false;
		}

		if (!m_aoBuffer.Init(WINDOW_WIDTH, WINDOW_HEIGHT, false, GL_R32F)) {
			return false;
		}

		if (!m_blurBuffer.Init(WINDOW_WIDTH, WINDOW_HEIGHT, false, GL_R32F)) {
			return false;
		}

		bar = TwNewBar("OGLDEV");

		m_pGameCamera->AddToATB(bar);

		TwAddSeparator(bar, "", NULL);

		TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation, " axisz=-z ");

		TwAddSeparator(bar, "", NULL);

		float refresh = 0.1f;
		TwSetParam(bar, NULL, "refresh", TW_PARAM_FLOAT, 1, &refresh);

		TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with OGLDEV.' "); // Message added to the help bar.

		TwAddVarRO(bar, "GL Major Version", TW_TYPE_INT32, &gGLMajorVersion, " label='Major version of GL' ");

		return true;
	}

	void Run()
	{
		OgldevBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		m_pGameCamera->OnRender();

		m_pipeline.SetCamera(*m_pGameCamera);

		GeometryPass();

		SSAOPass();

		BlurPass();

		LightingPass();

		RenderFPS();

		CalcFPS();

		OgldevBackendSwapBuffers();
	}

	void GeometryPass()
	{
		m_geomPassTech.Enable();

		m_depthBuffer.BindForWriting();

		glClear(GL_DEPTH_BUFFER_BIT);

		m_pipeline.Orient(m_mesh.GetOrientation());
		m_geomPassTech.SetWVP(m_pipeline.GetWVPTrans());
		m_mesh.Render();
	}

	void SSAOPass()
	{
		m_SSAOTech.Enable();
		m_SSAOTech.BindDepthBuffer(m_depthBuffer);

		m_aoBuffer.BindForWriting();

		glClear(GL_COLOR_BUFFER_BIT);

		m_quad.Render();
	}

	void BlurPass()
	{
		m_blurTech.Enable();

		m_blurTech.BindInputBuffer(m_aoBuffer);

		m_blurBuffer.BindForWriting();

		glClear(GL_COLOR_BUFFER_BIT);

		m_quad.Render();
	}

	void LightingPass()
	{
		m_lightingTech.Enable();
		m_lightingTech.SetShaderType(m_shaderType);
		m_lightingTech.BindAOBuffer(m_blurBuffer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pipeline.Orient(m_mesh.GetOrientation());
		m_lightingTech.SetWVP(m_pipeline.GetWVPTrans());
		m_lightingTech.SetWorldMatrix(m_pipeline.GetWorldTrans());
		m_mesh.Render();
	}

	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState)
	{
		if (OgldevKeyState == OGLDEV_KEY_STATE_PRESS) {
			if (m_atb.KeyboardCB(OgldevKey)) {
				return;
			}
		}

		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			OgldevBackendLeaveMainLoop();
			break;
		case OGLDEV_KEY_A:
			m_shaderType++;
			m_shaderType = m_shaderType % 3;
			break;
		default:
			m_pGameCamera->OnKeyboard(OgldevKey);
		}
	}

	virtual void PassiveMouseCB(int x, int y)
	{
		if (!m_atb.PassiveMouseCB(x, y)) {
			m_pGameCamera->OnMouse(x, y);
		}
	}

	virtual void MouseCB(OGLDEV_MOUSE Button, OGLDEV_KEY_STATE State, int x, int y)
	{
		m_atb.MouseCB(Button, State, x, y);
	}

private:

	SSAOTechnique m_SSAOTech;
	GeomPassTech m_geomPassTech;
	LightingTechnique m_lightingTech;
	BlurTech m_blurTech;
	Camera* m_pGameCamera;
	Mesh m_mesh;
	Mesh m_quad;
	PersProjInfo m_persProjInfo;
	Pipeline m_pipeline;
	IOBuffer m_depthBuffer;
	IOBuffer m_aoBuffer;
	IOBuffer m_blurBuffer;
	DirectionalLight m_directionalLight;
	int m_shaderType;
	ATB m_atb;
	TwBar *bar;
};

int main(int argc, char** argv)
{
	OgldevBackendInit(OGLDEV_BACKEND_TYPE_GLFW, argc, argv, true, false);

	if (!OgldevBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "OGLDEV Engine")) {
		OgldevBackendTerminate();
		return 1;
	}

	SRANDOM;

	Main* pApp = new Main();

	if (!pApp->Init()) {
		delete pApp;
		OgldevBackendTerminate();
		return 1;
	}

	pApp->Run();

	delete pApp;

	OgldevBackendTerminate();

	return 0;
}