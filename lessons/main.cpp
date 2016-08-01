#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif
#include <sys/types.h>

#include "engine_common.h"
#include "ogldev_util.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "skinning_technique.h"
#include "glut_backend.h"
#include "mesh.h"

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

class Main : public ICallbacks
{
public:

	Main()
	{
		m_pGameCamera = NULL;
		m_pEffect = NULL;
		m_scale = 0.0f;
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
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pEffect);
		SAFE_DELETE(m_pGameCamera);
	}

	bool Init()
	{
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
   	
		m_startTime = GetCurrentTimeMillis();

		return true;
	}

	void Run()
	{
		GLUTBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		m_scale += 0.005f;

		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pEffect->Enable();

		vector<Matrix4f> Transforms;

		float RunningTime = (float)((double)GetCurrentTimeMillis() - (double)m_startTime) / 1000.0f;

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

	SkinningTechnique* m_pEffect;
	Camera* m_pGameCamera;
	float m_scale;
	DirectionalLight m_directionalLight;
	Mesh m_mesh;
	Vector3f m_position;
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