#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif

#include "engine_common.h"
#include "ogldev_util.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "shadow_volume_technique.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#include "null_technique.h"

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

class Main : public ICallbacks
{
public:

	Main()
	{
		m_pGameCamera = NULL;
		m_scale = 0.0f;
		m_pointLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_pointLight.Position = Vector3f(0.0f, 15.0f, 0.0f);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;

		m_boxPos = Vector3f(0.0f, 2.0f, 0.0);
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pGameCamera);
	}

	bool Init()
	{
		Vector3f Pos(0.0f, 2.0f, -7.0f);
		Vector3f Target(0.0f, 0.0f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		if (!m_nullTech.Init()) {
			printf("Error initializing the null technique\n");
			return false;
		}

		if (!m_ShadowVolTech.Init()) {
			printf("Error initializing the shadow volume technique\n");
			return false;
		}

		if (!m_LightingTech.Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_LightingTech.Enable();

		m_LightingTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_LightingTech.SetPointLights(1, &m_pointLight);
		m_LightingTech.SetMatSpecularIntensity(0.0f);
		m_LightingTech.SetMatSpecularPower(0);

		if (!m_box.LoadMesh("thirdparty/content/box.obj", true)) {
			printf("Mesh load failed\n");
			return false;
		}
   	
		m_startTime = GetCurrentTimeMillis();

		if (!m_quad.LoadMesh("thirdparty/content/quad.obj", false)) {
			return false;
		}

		m_pGroundTex = new Texture(GL_TEXTURE_2D, "thirdparty/content/test.png");

		if (!m_pGroundTex->Load()) {
			return false;
		}

		return true;
	}

	void Run()
	{
		GLUTBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		m_scale += 0.1f;

		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		RenderSceneIntoDepth();

		glEnable(GL_STENCIL_TEST);

		RenderShadowVolIntoStencil();

		RenderShadowedScene();

		glDisable(GL_STENCIL_TEST);

		RenderAmbientLight();

		glutSwapBuffers();
	}

	virtual void IdleCB()
	{
		RenderSceneCB();
	}

	virtual void SpecialKeyboardCB(int Key, int x, int y)
	{
		m_pGameCamera->OnKeyboard(Key);
	}

	virtual void KeyboardCB(unsigned char Key, int x, int y)
	{
		switch (Key) {
		case 27:
			glutLeaveMainLoop();
			break;
		}
	}

	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}

	virtual void MouseCB(int Button, int State, int x, int y)
	{
	}

private:

	void RenderSceneIntoDepth()
	{
		glDrawBuffer(GL_NONE);
		glDepthMask(GL_TRUE);

		m_nullTech.Enable();

		Pipeline p;

		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);

		p.WorldPos(m_boxPos);
		p.Rotate(0, m_scale, 0);
		m_nullTech.SetWVP(p.GetWVPTrans());
		m_box.Render();

		p.Scale(10.0f, 10.0f, 10.0f);
		p.WorldPos(0.0f, 0.0f, 0.0f);
		p.Rotate(90.0f, 0.0f, 0.0f);
		m_nullTech.SetWVP(p.GetWVPTrans());
		m_quad.Render();
	}

	void RenderShadowVolIntoStencil()
	{
		glDrawBuffer(GL_NONE);
		glDepthMask(GL_FALSE);

		glDisable(GL_CULL_FACE);

		// We need the stencil test to be enabled but we want it
		// to succeed always. Only the depth test matters.
		glStencilFunc(GL_ALWAYS, 0, 0xff);

		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		m_ShadowVolTech.Enable();

		m_ShadowVolTech.SetLightPos(m_pointLight.Position);

		Pipeline p;
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		p.WorldPos(m_boxPos);
		p.Rotate(0, m_scale, 0);
		m_ShadowVolTech.SetWorldMatrix(p.GetWorldTrans());
		m_ShadowVolTech.SetVP(p.GetVPTrans());

		m_box.Render();

		glEnable(GL_CULL_FACE);
	}

	void RenderShadowedScene()
	{
		glDrawBuffer(GL_BACK);

		// prevent update to the stencil buffer
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_EQUAL, 0x0, 0xFF);

		m_LightingTech.Enable();

		m_pointLight.AmbientIntensity = 0.0f;
		m_pointLight.DiffuseIntensity = 0.8f;

		m_LightingTech.SetPointLights(1, &m_pointLight);

		Pipeline p;
		p.SetPerspectiveProj(m_persProjInfo);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());

		p.WorldPos(m_boxPos);
		p.Rotate(0, m_scale, 0);
		m_LightingTech.SetWVP(p.GetWVPTrans());
		m_LightingTech.SetWorldMatrix(p.GetWorldTrans());
		m_box.Render();

		p.Scale(10.0f, 10.0f, 10.0f);
		p.WorldPos(0.0f, 0.0f, 0.0f);
		p.Rotate(90.0f, 0.0f, 0.0f);
		m_LightingTech.SetWVP(p.GetWVPTrans());
		m_LightingTech.SetWorldMatrix(p.GetWorldTrans());
		m_pGroundTex->Bind(COLOR_TEXTURE_UNIT);
		m_quad.Render();
	}

	void RenderAmbientLight()
	{
		glDrawBuffer(GL_BACK);
		glDepthMask(GL_TRUE);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		m_LightingTech.Enable();

		m_pointLight.AmbientIntensity = 0.2f;
		m_pointLight.DiffuseIntensity = 0.0f;

		m_LightingTech.SetPointLights(1, &m_pointLight);

		m_pGroundTex->Bind(COLOR_TEXTURE_UNIT);

		Pipeline p;
		p.SetPerspectiveProj(m_persProjInfo);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());

		p.WorldPos(m_boxPos);
		p.Rotate(0, m_scale, 0);
		m_LightingTech.SetWVP(p.GetWVPTrans());
		m_LightingTech.SetWorldMatrix(p.GetWorldTrans());
		m_box.Render();

		p.Scale(10.0f, 10.0f, 10.0f);
		p.WorldPos(0.0f, 0.0f, 0.0f);
		p.Rotate(90.0f, 0.0f, 0.0f);
		m_LightingTech.SetWVP(p.GetWVPTrans());
		m_LightingTech.SetWorldMatrix(p.GetWorldTrans());

		m_quad.Render();

		glDisable(GL_BLEND);
	}

	ShadowVolumeTechnique m_ShadowVolTech;
	LightingTechnique m_LightingTech;
	NullTechnique m_nullTech;
	Camera* m_pGameCamera;
	float m_scale;
	PointLight m_pointLight;
	Vector3f m_boxPos;
	Mesh m_box;
	Mesh m_quad;
	Texture* m_pGroundTex;
	PersProjInfo m_persProjInfo;

	long long m_startTime;
};

int main(int argc, char** argv)
{
	Magick::InitializeMagick(*argv);
	GLUTBackendInit(argc, argv);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "OGLDev Tutotials")) {
		return 1;
	}

	SRANDOM;

	Main* pApp = new Main();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}