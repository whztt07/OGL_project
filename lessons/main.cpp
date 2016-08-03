#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

#include <float.h>

#include "ogldev_engine_common.h"
#include "ogldev_util.h"
#include "ogldev_app.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "ogldev_glut_backend.h"
#include "ogldev_basic_mesh.h"
#include "shadow_map_technique.h"
#include "shadow_cube_map_fbo.h"

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

struct CameraDirection
{
	GLenum CubemapFace;
	Vector3f Target;
	Vector3f Up;
};

CameraDirection gCameraDirections[NUM_OF_LAYERS] =
{
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Vector3f(1.0f, 0.0f, 0.0f),  Vector3f(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Vector3f(-1.0f, 0.0f, 0.0f), Vector3f(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Vector3f(0.0f, 1.0f, 0.0f),  Vector3f(0.0f, 0.0f, -1.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Vector3f(0.0f, -1.0f, 0.0f), Vector3f(0.0f, 0.0f, 1.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Vector3f(0.0f, 0.0f, 1.0f),  Vector3f(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Vector3f(0.0f, 0.0f, -1.0f), Vector3f(0.0f, -1.0f, 0.0f) }
};

class Main : public ICallbacks, public OgldevApp
{
public:

	Main()
	{
		m_pGameCamera = NULL;
		m_scale = 0.0f;
		m_pointLight.AmbientIntensity = 0.1f;
		m_pointLight.DiffuseIntensity = 0.9f;
		m_pointLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_pointLight.Attenuation.Linear = 0.0f;
		m_pointLight.Position = Vector3f(0.0, 5.0, 0.0f);

		m_persProjInfo.FOV = 90.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 100.0f;
	}

	virtual ~Main()
	{
		SAFE_DELETE(m_pGameCamera);
	}

	bool Init()
	{
		Vector3f Pos(0.0f, 8.0f, -10.0f);
		Vector3f Target(0.0f, -0.5f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
			printf("Error initializing the shadow map FBO\n");
			return false;
		}

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		if (!m_lightingEffect.Init()) {
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_lightingEffect.Enable();

		m_lightingEffect.SetColorTextureUnit(COLOR_TEXTURE_UNIT_INDEX);
		m_lightingEffect.SetShadowMapTextureUnit(SHADOW_TEXTURE_UNIT_INDEX);
		m_lightingEffect.SetPointLight(m_pointLight);

		if (!m_shadowMapEffect.Init()) {
			printf("Error initializing the shadow map technique\n");
			return false;
		}

		m_shadowMapEffect.Enable();
		m_shadowMapEffect.SetLightWorldPos(m_pointLight.Position);

		if (!m_quad.LoadMesh("thirdparty/content/quad.obj")) {
			return false;
		}

		// Set the orientation of the two quads
		m_quad1Orientation.m_scale = Vector3f(10.0f, 10.0f, 10.0f);
		m_quad1Orientation.m_rotation = Vector3f(90.0f, 0.0f, 0.0f);

		m_quad2Orientation.m_scale = Vector3f(10.0f, 10.0f, 10.0f);
		m_quad2Orientation.m_pos = Vector3f(0.0f, 0.0f, 7.0f);

		m_pGroundTex = new Texture(GL_TEXTURE_2D, "thirdparty/content/test.png");

		if (!m_pGroundTex->Load()) {
			return false;
		}

		if (!m_mesh.LoadMesh("thirdparty/content/sphere.obj")) {
			return false;
		}

		m_mesh1Orientation.m_pos = Vector3f(0.0f, 3.0f, 0.0f);
		m_mesh2Orientation.m_pos = Vector3f(0.0f, 5.0f, 3.0f);

		glEnable(GL_TEXTURE_CUBE_MAP);

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

		m_shadowMapEffect.Enable();

		PersProjInfo ProjInfo;
		ProjInfo.FOV = 90.0f;
		ProjInfo.Height = WINDOW_HEIGHT;
		ProjInfo.Width = WINDOW_WIDTH;
		ProjInfo.zNear = 1.0f;
		ProjInfo.zFar = 100.0f;

		Pipeline p;
		p.SetPerspectiveProj(m_persProjInfo);

		glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

		for (uint i = 0; i < NUM_OF_LAYERS; i++) {
			m_shadowMapFBO.BindForWriting(gCameraDirections[i].CubemapFace);
			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			p.SetCamera(m_pointLight.Position, gCameraDirections[i].Target, gCameraDirections[i].Up);

			p.Orient(m_mesh1Orientation);
			m_shadowMapEffect.SetWorld(p.GetWorldTrans());
			m_shadowMapEffect.SetWVP(p.GetWVPTrans());
			m_mesh.Render();

			p.Orient(m_mesh2Orientation);
			m_shadowMapEffect.SetWorld(p.GetWorldTrans());
			m_shadowMapEffect.SetWVP(p.GetWVPTrans());
			m_mesh.Render();
		}
	}

	void RenderPass()
	{
		glCullFace(GL_BACK);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_lightingEffect.Enable();
		m_shadowMapFBO.BindForReading(SHADOW_TEXTURE_UNIT);
		m_lightingEffect.SetEyeWorldPos(m_pGameCamera->GetPos());

		Pipeline p;
		p.SetPerspectiveProj(m_persProjInfo);
		p.SetCamera(*m_pGameCamera);

		// Render the quads
		m_pGroundTex->Bind(COLOR_TEXTURE_UNIT);
		p.Orient(m_quad1Orientation);
		m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
		m_lightingEffect.SetWVP(p.GetWVPTrans());
		m_quad.Render();

		p.Orient(m_quad2Orientation);
		m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
		m_lightingEffect.SetWVP(p.GetWVPTrans());
		m_quad.Render();

		// Render the meshes
		p.Orient(m_mesh1Orientation);
		m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
		m_lightingEffect.SetWVP(p.GetWVPTrans());
		m_mesh.Render();

		p.Orient(m_mesh2Orientation);
		m_lightingEffect.SetWorldMatrix(p.GetWorldTrans());
		m_lightingEffect.SetWVP(p.GetWVPTrans());
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

	LightingTechnique m_lightingEffect;
	ShadowMapTechnique m_shadowMapEffect;
	Camera* m_pGameCamera;
	float m_scale;
	PointLight m_pointLight;
	BasicMesh m_mesh;
	BasicMesh m_quad;
	Orientation m_quad1Orientation;
	Orientation m_quad2Orientation;
	Orientation m_mesh1Orientation;
	Orientation m_mesh2Orientation;
	PersProjInfo m_persProjInfo;
	Texture* m_pGroundTex;
	ShadowCubeMapFBO m_shadowMapFBO;
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