#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

#include "ogldev_engine_common.h"
#include "ogldev_app.h"
#include "ogldev_backend.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_texture.h"
#include "lighting_technique.h"
#include "ogldev_glut_backend.h"
#include "ogldev_basic_mesh.h"
#include "shadow_map_technique.h"
#include "shadow_map_fbo.h"

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

class Main : public ICallbacks, public OgldevApp
{
public:

	Main()
	{
		m_pGameCamera = NULL;
		m_pLightingEffect = NULL;
		m_pShadowMapEffect = NULL;
		m_scale = 0.0f;
		m_spotLight.AmbientIntensity = 0.1f;
		m_spotLight.DiffuseIntensity = 0.9f;
		m_spotLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_spotLight.Attenuation.Linear = 0.01f;
		m_spotLight.Position = Vector3f(-20.0, 20.0, 1.0f);
		m_spotLight.Direction = Vector3f(1.0f, -1.0f, 0.0f);
		m_spotLight.Cutoff = 20.0f;

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pLightingEffect);
		SAFE_DELETE(m_pShadowMapEffect);
		SAFE_DELETE(m_pGameCamera);
	}

	bool Init()
	{
		Vector3f Pos(3.0f, 8.0f, -10.0f);
		Vector3f Target(0.0f, -0.5f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
			return false;
		}

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_pLightingEffect = new LightingTechnique();

		if (!m_pLightingEffect->Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pLightingEffect->Enable();

		m_pLightingEffect->SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_pLightingEffect->SetShadowMapTextureUnit(SHADOW_TEXTURE_UNIT_INDEX);
		m_pLightingEffect->SetSpotLights(1, &m_spotLight);
		m_pLightingEffect->SetShadowMapSize((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT);

		m_pShadowMapEffect = new ShadowMapTechnique();

		if (!m_pShadowMapEffect->Init()) {
			printf("Error initializing the shadow map technique\n");
			return false;
		}

		if (!m_quad.LoadMesh("thirdparty/content/quad.obj")) {
			return false;
		}

		m_pGroundTex = new Texture(GL_TEXTURE_2D, "thirdparty/content/test.png");

		if (!m_pGroundTex->Load()) {
			return false;
		}

		if (!m_mesh.LoadMesh("thirdparty/content/phoenix_ugv.md2")) {
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
		CalcFPS();

		m_scale += 0.05f;

		m_pGameCamera->OnRender();

		ShadowMapPass();
		RenderPass();

		RenderFPS();

		glutSwapBuffers();
	}

	void ShadowMapPass()
	{
		glCullFace(GL_FRONT);

		m_shadowMapFBO.BindForWriting();

		glClear(GL_DEPTH_BUFFER_BIT);

		m_pShadowMapEffect->Enable();

		Pipeline p;
		p.Scale(0.1f, 0.1f, 0.1f);
		p.Rotate(0.0f, m_scale, 0.0f);
		p.WorldPos(0.0f, 0.0f, 3.0f);
		p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
		p.SetPerspectiveProj(m_persProjInfo);
		m_pShadowMapEffect->SetWVP(p.GetWVPTrans());
		m_mesh.Render();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderPass()
	{
		glCullFace(GL_BACK);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_pLightingEffect->Enable();

		m_shadowMapFBO.BindForReading(GL_TEXTURE1);

		Pipeline p;
		p.SetPerspectiveProj(m_persProjInfo);
		p.Scale(10.0f, 10.0f, 10.0f);
		p.WorldPos(0.0f, 0.0f, 1.0f);
		p.Rotate(90.0f, 0.0f, 0.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());

		m_pLightingEffect->SetWVP(p.GetWVPTrans());
		m_pLightingEffect->SetWorldMatrix(p.GetWorldTrans());
		p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
		m_pLightingEffect->SetLightWVP(p.GetWVPTrans());
		m_pLightingEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
		m_pGroundTex->Bind(GL_TEXTURE0);
		m_quad.Render();

		p.Scale(0.1f, 0.1f, 0.1f);
		p.Rotate(0.0f, m_scale, 0.0f);
		p.WorldPos(0.0f, 0.0f, 3.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		m_pLightingEffect->SetWVP(p.GetWVPTrans());
		m_pLightingEffect->SetWorldMatrix(p.GetWorldTrans());
		p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
		m_pLightingEffect->SetLightWVP(p.GetWVPTrans());

		m_mesh.Render();
	}

	virtual void KeyboardCB(OGLDEV_KEY OgldevKey)
	{
		switch (OgldevKey) {
		//case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
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

	LightingTechnique* m_pLightingEffect;
	ShadowMapTechnique* m_pShadowMapEffect;
	Camera* m_pGameCamera;
	float m_scale;
	SpotLight m_spotLight;
	BasicMesh m_mesh;
	BasicMesh m_quad;
	PersProjInfo m_persProjInfo;
	Texture* m_pGroundTex;
	ShadowMapFBO m_shadowMapFBO;
};

int main(int argc, char** argv)
{
	Magick::InitializeMagick(*argv);
	GLUTBackendInit(argc, argv, true, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "OGLDEV Tutorials")) {
		return 1;
	}

	Main App;

	if (!App.Init()) {
		return 1;
	}

	App.Run();

	return 0;
}