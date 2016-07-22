#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "engine_common.h"
#include "util.h"
#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class Main : public ICallbacks
{
public:

	Main()
	{
		m_pGameCamera = NULL;
		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.1f;
		m_directionalLight.DiffuseIntensity = 0.9f;
		m_directionalLight.Direction = Vector3f(0.0f, 0.0, 1.0);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;

		m_tessellationLevel = 5.0f;
		m_isWireframe = false;
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(m_pMesh);
	}

	bool Init()
	{
		Vector3f Pos(0.0f, 1.5f, -6.5f);
		Vector3f Target(0.0f, -0.2f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		if (!m_lightingEffect.Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		GLint MaxPatchVertices = 0;
		glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
		printf("Max supported patch vertices %d\n", MaxPatchVertices);
		glPatchParameteri(GL_PATCH_VERTICES, 3);

		m_lightingEffect.Enable();
		m_lightingEffect.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_lightingEffect.SetDirectionalLight(m_directionalLight);
		m_pMesh = new Mesh();

		return m_pMesh->LoadMesh("thirdparty/content/monkey.obj");
	}

	void Run()
	{
		GLUTBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pGameCamera->OnRender();

		Pipeline p;
		p.WorldPos(-3.0f, 0.0f, 0.0f);
		p.Scale(2.0f, 2.0f, 2.0f);
		p.Rotate(-90.0f, 15.0f, 0.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		m_lightingEffect.SetEyeWorldPos(m_pGameCamera->GetPos());

		m_lightingEffect.SetVP(p.GetVPTrans());
		m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
		m_lightingEffect.SetTessellationLevel(m_tessellationLevel);
		m_pMesh->Render(NULL);

		p.WorldPos(3.0f, 0.0f, 0.0f);
		p.Rotate(-90.0f, -15.0f, 0.0f);
		m_lightingEffect.SetVP(p.GetVPTrans());
		m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
		m_lightingEffect.SetTessellationLevel(1.0f);
		m_pMesh->Render(NULL);
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

			case '+':
				m_tessellationLevel += 1.0f;
				break;

			case '-':
				if (m_tessellationLevel >= 2.0f) {
					m_tessellationLevel -= 1.0f;
				}
				break;

			case 'z':
				m_isWireframe = !m_isWireframe;

				if (m_isWireframe) {
					glPolygonMode(GL_FRONT, GL_LINE);
				}
				else {
					glPolygonMode(GL_FRONT, GL_FILL);
				}
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

	LightingTechnique m_lightingEffect;
	Camera* m_pGameCamera;
	DirectionalLight m_directionalLight;
	Mesh* m_pMesh;
	PersProjInfo m_persProjInfo;
	float m_tessellationLevel;
	bool m_isWireframe;
};

int main(int argc, char** argv)
{
	GLUTBackendInit(argc, argv);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "OGLDev Tutorials")) {
		return 1;
	}

	Main* pApp = new Main();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}