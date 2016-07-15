#include <stdlib.h>
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
#include "particle_system.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class Main : public ICallbacks
{
public:

	Main()
	{
		m_pLightingTechnique = NULL;
		m_pGameCamera = NULL;
		m_pGround = NULL;
		m_pTexture = NULL;
		m_pNormalMap = NULL;

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
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pLightingTechnique);
		SAFE_DELETE(m_pGameCamera);
		SAFE_DELETE(m_pGround);
		SAFE_DELETE(m_pTexture);
		SAFE_DELETE(m_pNormalMap);
	}

	bool Init()
	{
		Vector3f Pos(0.0f, 0.4f, -0.5f);
		Vector3f Target(0.0f, 0.2f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_pLightingTechnique = new LightingTechnique();

		if (!m_pLightingTechnique->Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pLightingTechnique->Enable();
		m_pLightingTechnique->SetDirectionalLight(m_dirLight);
		m_pLightingTechnique->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_pLightingTechnique->SetNormalMapTextureUnit(NORMAL_TEXTURE_UNIT_INDEX);

		m_pGround = new Mesh();
		if (!m_pGround->LoadMesh("thirdparty/content/quad.obj")) {
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

		Vector3f ParticleSystemPos = Vector3f(0.0f, 0.0f, 1.0f);

		return m_particleSystem.InitParticleSystem(ParticleSystemPos);
	}

	void Run()
	{
		GLUTBackendRun(this);
	}

	virtual void RenderSceneCB()
	{
		long long TimeNowMillis = GetCurrentTimeMillis();
		assert(TimeNowMillis >= m_currentTimeMillis);
		unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
		m_currentTimeMillis = TimeNowMillis;
		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pLightingTechnique->Enable();

		m_pTexture->Bind(COLOR_TEXTURE_UNIT);
		m_pNormalMap->Bind(NORMAL_TEXTURE_UNIT);

		Pipeline p;
		p.Scale(20.0f, 20.0f, 1.0f);
		p.Rotate(90.0f, 0.0, 0.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);

		m_pLightingTechnique->SetWVP(p.GetWVPTrans());
		m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());

		m_pGround->Render();

		m_particleSystem.Render(DeltaTimeMillis, p.GetVPTrans(), m_pGameCamera->GetPos());

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

 private:

	 long long m_currentTimeMillis;
	 LightingTechnique* m_pLightingTechnique;
	 Camera* m_pGameCamera;
	 DirectionalLight m_dirLight;
	 Mesh* m_pGround;
	 Texture* m_pTexture;
	 Texture* m_pNormalMap;
	 PersProjInfo m_persProjInfo;
	 ParticleSystem m_particleSystem;
};

int main(int argc, char** argv)
{
	SRANDOM;

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