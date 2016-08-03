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
#include "lighting_technique.h"
#include "ogldev_backend.h"
#include "ogldev_camera.h"
#include "mesh.h"

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

class Main : public ICallbacks, public OgldevApp
{
public:

	Main()
	{
		m_pGameCamera = NULL;
		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.66f;
		m_directionalLight.DiffuseIntensity = 1.0f;
		m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 1000.0f;

		m_pipeline.SetPerspectiveProj(m_persProjInfo);
		m_pipeline.WorldPos(Vector3f(0.0f, 0.0f, 0.0f));
		m_pipeline.Scale(0.1f, 0.1f, 0.1f);
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pGameCamera);
	}

	bool Init()
	{
		Vector3f Pos(0.0f, 23.0f, -5.0f);
		Vector3f Target(-1.0f, 0.0f, 0.1f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		if (!m_LightingTech.Init()) {
			OGLDEV_ERROR("Error initializing the lighting technique\n");
			return false;
		}

		m_LightingTech.Enable();

		m_LightingTech.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_LightingTech.SetDirectionalLight(m_directionalLight);
		m_LightingTech.SetMatSpecularIntensity(0.0f);
		m_LightingTech.SetMatSpecularPower(0);

		if (!m_mesh.LoadMesh("thirdparty/content/crytek_sponza/sponza.obj")) {
			return false;
		}

		return true;
	}

	void Run()
	{
		OgldevBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_LightingTech.SetEyeWorldPos(m_pGameCamera->GetPos());

		m_pipeline.SetCamera(*m_pGameCamera);

		m_LightingTech.SetWVP(m_pipeline.GetWVPTrans());
		m_LightingTech.SetWorldMatrix(m_pipeline.GetWorldTrans());

		m_mesh.Render();

		// RenderFPS();     
		CalcFPS();

		OgldevBackendSwapBuffers();
	}

	virtual void KeyboardCB(OGLDEV_KEY OgldevKey)
	{
		switch (OgldevKey) {
		//case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			OgldevBackendLeaveMainLoop();
			break;
		default:
			m_pGameCamera->OnKeyboard(OgldevKey);
		}
	}

	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}

private:

	LightingTechnique m_LightingTech;
	Camera* m_pGameCamera;
	DirectionalLight m_directionalLight;
	Mesh m_mesh;
	PersProjInfo m_persProjInfo;
	Pipeline m_pipeline;
};

int main(int argc, char** argv)
{
	Magick::InitializeMagick(*argv);

	OgldevBackendInit(OGLDEV_BACKEND_TYPE_GLFW, argc, argv, true, false);

	if (!OgldevBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "OGLDEV Tutorials")) {
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