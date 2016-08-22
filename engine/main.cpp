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
#include "ogldev_basic_lighting.h"
#include "ogldev_backend.h"
#include "ogldev_atb.h"
#include "skybox.h"
#include "mesh.h"
#include "particle_system.h"

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
		m_pGroundTex = NULL;
		m_pTexture = NULL;
		m_pNormalMap = NULL;
		m_pSkyBox = NULL;

		m_dirLight.Name = "DirLight1";
		m_dirLight.AmbientIntensity = 0.2f;
		m_dirLight.DiffuseIntensity = 0.8f;
		m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_dirLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;

		m_currentTimeMillis = GetCurrentTimeMillis();

		glGetIntegerv(GL_MAJOR_VERSION, &gGLMajorVersion);
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(m_pGroundTex);
		SAFE_DELETE(m_pTexture);
		SAFE_DELETE(m_pNormalMap);
		SAFE_DELETE(m_pSkyBox);
	}

	bool Init()
	{
		if (!m_atb.Init()) {
			return false;
		}

		Vector3f Pos(0.0f, 0.4f, -0.5f);
		Vector3f Target(0.0f, 0.2f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		if (!m_LightingTech.Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_LightingTech.Enable();
		m_LightingTech.SetDirectionalLight(m_dirLight);
		m_LightingTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);

		m_pGroundTex = new Mesh();

		if (!m_pGroundTex->LoadMesh("thirdparty/content/quad.obj")) {
			return false;
		}

		m_pTexture = new Texture(GL_TEXTURE_2D, "thirdparty/content/bricks.jpg");

		if (!m_pTexture->Load()) {
			return false;
		}

		m_pTexture->Bind(COLOR_TEXTURE_UNIT);

		m_pNormalMap = new Texture(GL_TEXTURE_2D, "thirdparty/content/normal_map.jpg");

		if (!m_pNormalMap->Load()) {
			return false;
		}

		m_pSkyBox = new SkyBox(m_pGameCamera, m_persProjInfo);

		if (!m_pSkyBox->Init(".",
			"thirdparty/content/sp3right.jpg",
			"thirdparty/content/sp3left.jpg",
			"thirdparty/content/sp3top.jpg",
			"thirdparty/content/sp3bot.jpg",
			"thirdparty/content/sp3front.jpg",
			"thirdparty/content/sp3back.jpg")) {
			return false;
		}

		bar = TwNewBar("OGLDEV");

		m_pGameCamera->AddToATB(bar);

		TwAddSeparator(bar, "", NULL);

		TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4F, &g_Rotation, " axisz=-z ");

		TwAddSeparator(bar, "", NULL);

		m_dirLight.AddToATB(bar);

		float refresh = 0.1f;
		TwSetParam(bar, NULL, "refresh", TW_PARAM_FLOAT, 1, &refresh);

		TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with OGLDEV.' "); // Message added to the help bar.

		TwAddVarRO(bar, "GL Major Version", TW_TYPE_INT32, &gGLMajorVersion, " label='Major version of GL' ");

		Vector3f ParticleSystemPos = Vector3f(0.0f, 0.0f, 1.0f);

		return m_particleSystem.InitParticleSystem(ParticleSystemPos);
	}

	void Run()
	{
		OgldevBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		long long TimeNowMillis = GetCurrentTimeMillis();
		assert(TimeNowMillis >= m_currentTimeMillis);
		unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
		m_currentTimeMillis = TimeNowMillis;
		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_LightingTech.Enable();

		m_pTexture->Bind(COLOR_TEXTURE_UNIT);
		m_pNormalMap->Bind(NORMAL_TEXTURE_UNIT);

		Pipeline p;
		p.Scale(20.0f, 20.0f, 1.0f);
		p.Rotate(90.0f, 0.0, 0.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);

		m_LightingTech.SetWVP(p.GetWVPTrans());
		m_LightingTech.SetWorldMatrix(p.GetWorldTrans());

		m_pGroundTex->Render();

		m_particleSystem.Render(DeltaTimeMillis, p.GetVPTrans(), m_pGameCamera->GetPos());

		m_pSkyBox->Render();

		OgldevBackendSwapBuffers();
	}

	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState)
	{
		if (OgldevKeyState == OGLDEV_KEY_STATE_PRESS) {
			if (m_atb.KeyboardCB(OgldevKey)) {
				return;
			}
		}

		switch (OgldevKey) {
		case OGLDEV_KEY_A:
		{
			int Pos[2], Size[2];
			TwGetParam(bar, NULL, "position", TW_PARAM_INT32, 2, Pos);
			TwGetParam(bar, NULL, "size", TW_PARAM_INT32, 2, Size);
			OgldevBackendSetMousePos(Pos[0] + Size[0] / 2,
				Pos[1] + Size[1] / 2);
			break;
		}
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			OgldevBackendLeaveMainLoop();
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

	long long m_currentTimeMillis;
	BasicLightingTechnique m_LightingTech;
	Camera* m_pGameCamera;
	DirectionalLight m_dirLight;
	Mesh* m_pGroundTex;
	Texture* m_pTexture;
	Texture* m_pNormalMap;
	PersProjInfo m_persProjInfo;
	ParticleSystem m_particleSystem;
	ATB m_atb;
	TwBar *bar;
	SkyBox* m_pSkyBox;
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