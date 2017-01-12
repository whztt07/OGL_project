#pragma once

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
#include "ogldev_backend.h"
#include "ogldev_camera.h"
#include "ogldev_lights_common.h"
#include "ogldev_shadow_map_fbo.h"
#include "ogldev_atb.h"
#include "ogldev_io_buffer.h"
#include "ssao_technique.h"
#include "geom_pass_tech.h"
#include "blur_tech.h"
#include "mesh.h"
#include "lighting_technique.h"

#define WINDOW_WIDTH  1024
#define WINDOW_HEIGHT 1024

class Main : public ICallbacks, public OgldevApp
{
public:

	Main()
	{
	}

	virtual ~Main()
	{
	}

	bool Init()
	{
	}

	void Run()
	{
	}

	virtual void RenderSceneCB()
	{
	}

	void GeometryPass()
	{
	}

	void SSAOPass()
	{
	}

	void BlurPass()
	{
	}

	void LightingPass()
	{
	}

	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState)
	{
	}

	virtual void PassiveMouseCB(int x, int y)
	{
	}

	virtual void MouseCB(OGLDEV_MOUSE Button, OGLDEV_KEY_STATE State, int x, int y)
	{
	}

private:

	SSAOTechnique m_SSAOTech;
	GeomPassTech m_geomPassTech;
	LightingTechnique m_lightingTech;
	BlurTech m_blurTech;
	Camera* m_pGameCamera;
	Mesh m_mesh;
	Mesh m_quad;
	PersProjInfo m_persProjInfo;
	Pipeline m_pipeline;
	IOBuffer m_depthBuffer;
	IOBuffer m_aoBuffer;
	IOBuffer m_blurBuffer;
	DirectionalLight m_directionalLight;
	int m_shaderType;
	ATB m_atb;
	TwBar *bar;
};