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
#include "ogldev_skinned_mesh.h"
#include "skinning_technique.h"

using namespace std;

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
		m_pEffect = NULL;
		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.55f;
		m_directionalLight.DiffuseIntensity = 0.9f;
		m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;

		m_position = Vector3f(0.0f, 0.0f, 6.0f);

		glGetIntegerv(GL_MAJOR_VERSION, &gGLMajorVersion);
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(m_pEffect);
	}

	bool Init()
	{
		if (!m_atb.Init()) {
			return false;
		}

		Vector3f Pos(0.0f, 3.0f, -1.0f);
		Vector3f Target(0.0f, 0.0f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_pEffect = new SkinningTechnique();

		if (!m_pEffect->Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pEffect->Enable();

		m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_pEffect->SetDirectionalLight(m_directionalLight);
		m_pEffect->SetMatSpecularIntensity(0.0f);
		m_pEffect->SetMatSpecularPower(0);

		if (!m_mesh.LoadMesh("thirdparty/content/boblampclean.md5mesh")) {
			printf("Mesh load failed\n");
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

		Vector3f ParticleSystemPos = Vector3f(0.0f, 0.0f, 1.0f);

		return true;
	}

	void Run()
	{
		OgldevBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		CalcFPS();

		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pEffect->Enable();

		vector<Matrix4f> Transforms;

		float RunningTime = GetRunningTime();

		m_mesh.BoneTransform(RunningTime, Transforms);

		for (uint i = 0; i < Transforms.size(); i++) {
			m_pEffect->SetBoneTransform(i, Transforms[i]);
		}

		m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

		Pipeline p;
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		p.Scale(0.1f, 0.1f, 0.1f);

		Vector3f Pos(m_position);
		p.WorldPos(Pos);
		p.Rotate(270.0f, 180.0f, 0.0f);
		m_pEffect->SetWVP(p.GetWVPTrans());
		m_pEffect->SetWorldMatrix(p.GetWorldTrans());

		m_mesh.Render();

		RenderFPS();

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

	SkinningTechnique* m_pEffect;
	Camera* m_pGameCamera;
	DirectionalLight m_directionalLight;
	SkinnedMesh m_mesh;
	Vector3f m_position;
	PersProjInfo m_persProjInfo;
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