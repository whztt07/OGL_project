#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

#include "engine_common.h"
#include "util.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"

using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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

		m_positions[0] = Vector3f(-2.0f, 0.0f, 6.0f);
		m_positions[1] = Vector3f(0.0f, 0.0f, 6.0f);
		m_positions[2] = Vector3f(2.0f, 0.0f, 6.0f);
		m_positions[3] = Vector3f(4.0f, 0.0f, 6.0f);

		m_velocity[0] = 10.0f;
		m_velocity[1] = 15.0f;
		m_velocity[2] = 50.0f;
		m_velocity[3] = 20.0f;

		m_colors[0] = Vector4f(1.0f, 0.0, 0.0, 0.0f);
		m_colors[1] = Vector4f(0.0f, 1.0, 0.0, 0.0f);
		m_colors[2] = Vector4f(0.0f, 0.0, 1.0, 0.0f);
		m_colors[3] = Vector4f(1.0f, 0.0, 1.0, 0.0f);
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pEffect);
		SAFE_DELETE(m_pGameCamera);
	}

	bool Init()
	{
		Vector3f Pos(0.8f, 0.0f, 0.0f);
		Vector3f Target(0.0f, 0.0f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_pEffect = new LightingTechnique();

		if (!m_pEffect->Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pEffect->Enable();

		m_pEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_pEffect->SetDirectionalLight(m_directionalLight);
		m_pEffect->SetMatSpecularIntensity(0.0f);
		m_pEffect->SetMatSpecularPower(0);

		if (!m_mesh[0].LoadMesh("thirdparty/content/glfx/g.obj")) {
			return false;
		}

		if (!m_mesh[1].LoadMesh("thirdparty/content/glfx/l.obj")) {
			return false;
		}

		if (!m_mesh[2].LoadMesh("thirdparty/content/glfx/f.obj")) {
			return false;
		}

		if (!m_mesh[3].LoadMesh("thirdparty/content/glfx/x.obj")) {
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
		m_scale += 0.005f;

		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pEffect->Enable();
		m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());

		Pipeline p;
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		p.Scale(0.1f, 0.1f, 0.1f);

		Matrix4f WVPMatrics[1];
		Matrix4f WorldMatrices[1];

		for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_mesh); i++) {
			m_pEffect->SetColor(m_colors[i]);

			p.Rotate(0.0, m_scale * m_velocity[i], 0.0f);
			Vector3f Pos(m_positions[i]);
			p.WorldPos(Pos);
			m_pEffect->SetWVP(p.GetWVPTrans());
			m_pEffect->SetWorldMatrix(p.GetWorldTrans());

			m_mesh[i].Render(1, WVPMatrics, WorldMatrices);
		}

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

	virtual void KeyboardCB(unsigned char key, int x, int y)
	{
		switch (key) {
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

	LightingTechnique* m_pEffect;
	Camera* m_pGameCamera;
	float m_scale;
	DirectionalLight m_directionalLight;
	Mesh m_mesh[4];
	Vector3f m_positions[4];
	float m_velocity[4];
	Vector4f m_colors[4];
	PersProjInfo m_persProjInfo;
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