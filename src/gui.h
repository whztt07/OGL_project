#pragma once

#ifndef GUI_H
#define	GUI_H

#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogldev_pipeline.h"
#include "camera.h"
#include "ogldev_texture.h"
#include "lighting_technique.h"
#include "ogldev_glut_backend.h"
#include "ogldev_util.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;
	Vector3f m_normal;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
		m_normal = Vector3f(0.0f, 0.0f, 0.0f);
	}
};

class GUI : public ICallbacks
{
public:

	GUI()
	{
		m_pGameCamera = NULL;
		m_pTexture = NULL;
		m_pEffect = NULL;
		m_scale = 0.0f;
		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.2f;
		m_directionalLight.DiffuseIntensity = 0.0f;
		m_directionalLight.Direction = Vector3f(1.0f, 0.0f, 0.0f);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 0.1f;
		m_persProjInfo.zFar = 100.0f;
	}

	~GUI()
	{
		delete m_pEffect;
		delete m_pGameCamera;
		delete m_pTexture;
	}

	bool Init()
	{
		Vector3f Pos(0.0f, 0.0f, -3.0f);
		Vector3f Target(0.0f, 0.0f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);
		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		unsigned int Indices[] = { 0, 1, 5,
			0, 5, 4,
			3, 0, 4,
			3, 4, 7,
			2, 3, 7,
			2, 7, 6,
			1, 2, 6,
			1, 6, 5,
			3, 2, 1,
			3, 1, 0,
			4, 5, 6,
			4, 6, 7 };

		CreateIndexBuffer(Indices, sizeof(Indices));

		CreateVertexBuffer(Indices, ARRAY_SIZE_IN_ELEMENTS(Indices));

		m_pEffect = new LightingTechnique();

		if (!m_pEffect->Init())
		{
			printf("Error initializing the lighting technique\n");
			return false;
		}

		m_pEffect->Enable();

		m_pEffect->SetColorTextureUnit(0);

		m_pTexture = new Texture(GL_TEXTURE_2D, "thirdparty/content/test.png");

		if (!m_pTexture->Load()) {
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
		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT);

		m_scale += 0.03f;
		SpotLight sl[2];
		sl[0].DiffuseIntensity = 15.0f;
		sl[0].Color = Vector3f(1.0f, 1.0f, 0.7f);
		sl[0].Position = Vector3f(-0.0f, -1.9f, -0.0f);
		sl[0].Direction = Vector3f(sinf(m_scale), 0.0f, cosf(m_scale));
		sl[0].Attenuation.Linear = 0.1f;
		sl[0].Cutoff = 20.0f;

		sl[1].DiffuseIntensity = 5.0f;
		sl[1].Color = Vector3f(1.0f, 1.0f, 1.0f);
		sl[1].Position = m_pGameCamera->GetPos();
		sl[1].Direction = m_pGameCamera->GetTarget();
		sl[1].Attenuation.Linear = 0.1f;
		sl[1].Cutoff = 10.0f;

		m_pEffect->SetSpotLights(2, sl);

		PointLight pl[3];
		pl[0].DiffuseIntensity = 0.5f;
		pl[0].Color = Vector3f(1.0f, 0.0f, 0.0f);
		pl[0].Position = Vector3f(sinf(m_scale) * 10, 1.0f, cosf(m_scale) * 10);
		pl[0].Attenuation.Linear = 0.1f;

		pl[1].DiffuseIntensity = 0.5f;
		pl[1].Color = Vector3f(0.0f, 1.0f, 0.0f);
		pl[1].Position = Vector3f(sinf(m_scale + 2.1f) * 10, 1.0f, cosf(m_scale + 2.1f) * 10);
		pl[1].Attenuation.Linear = 0.1f;

		pl[2].DiffuseIntensity = 0.5f;
		pl[2].Color = Vector3f(0.0f, 0.0f, 1.0f);
		pl[2].Position = Vector3f(sinf(m_scale + 4.2f) * 10, 1.0f, cosf(m_scale + 4.2f) * 10);
		pl[2].Attenuation.Linear = 0.1f;

		m_pEffect->SetPointLights(3, pl);

		Pipeline p;
		p.Rotate(10.0f*m_scale, 10.0f*m_scale, 10.0f*m_scale);
		p.WorldPos(0.0f, 0.0f, 1.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);
		m_pEffect->SetWVP(p.GetWVPTrans());
		const Matrix4f& WorldTransformation = p.GetWorldTrans();
		m_pEffect->SetWorldMatrix(WorldTransformation);
		m_pEffect->SetDirectionalLight(m_directionalLight);
		m_pEffect->SetEyeWorldPos(m_pGameCamera->GetPos());
		m_pEffect->SetMatSpecularIntensity(0.0f);
		m_pEffect->SetMatSpecularPower(16);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		m_pTexture->Bind(GL_TEXTURE0);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glutSwapBuffers();
	}

	virtual void IdleCB()
	{
		RenderSceneCB();
	}

	void KeyboardCB(OGLDEV_KEY OgldevKey)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		case OGLDEV_KEY_a:
			m_directionalLight.AmbientIntensity += 0.05f;
			break;
		case OGLDEV_KEY_s:
			m_directionalLight.AmbientIntensity -= 0.05f;
			break;
		case OGLDEV_KEY_z:
			m_directionalLight.DiffuseIntensity += 0.05f;
			break;
		case OGLDEV_KEY_x:
			m_directionalLight.DiffuseIntensity -= 0.05f;
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

	void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
		Vertex* pVertices, unsigned int VertexCount) {
		for (unsigned int i = 0; i < IndexCount; i += 3) {
			unsigned int Index0 = pIndices[i];
			unsigned int Index1 = pIndices[i + 1];
			unsigned int Index2 = pIndices[i + 2];
			Vector3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
			Vector3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
			Vector3f Normal = v1.Cross(v2);
			Normal.Normalize();

			pVertices[Index0].m_normal += Normal;
			pVertices[Index1].m_normal += Normal;
			pVertices[Index2].m_normal += Normal;
		}

		for (unsigned int i = 0; i < VertexCount; i++) {
			pVertices[i].m_normal.Normalize();
		}
	}

	void CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
	{
		Vertex Vertices[8];
		Vertices[0] = Vertex(Vector3f(0.5, -0.5, -0.5), Vector2f(0.0, 0.0));
		Vertices[1] = Vertex(Vector3f(-0.5, -0.5, -0.5), Vector2f(1.0, 0.0));
		Vertices[2] = Vertex(Vector3f(-0.5, -0.5, 0.5), Vector2f(1.0, 1.0));
		Vertices[3] = Vertex(Vector3f(0.5, -0.5, 0.5), Vector2f(0.0, 1.0));
		Vertices[4] = Vertex(Vector3f(0.5, 0.5, -0.5), Vector2f(1.0, 0.0));
		Vertices[5] = Vertex(Vector3f(-0.5, 0.5, -0.5), Vector2f(1.0, 1.0));
		Vertices[6] = Vertex(Vector3f(-0.5, 0.5, 0.5), Vector2f(0.0, 1.0));
		Vertices[7] = Vertex(Vector3f(0.5, 0.5, 0.5), Vector2f(0.0, 0.0));

		unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);

		CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	}

	void CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes)
	{
		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
	}

	GLuint m_VBO;
	GLuint m_IBO;
	LightingTechnique* m_pEffect;
	Texture* m_pTexture;
	Camera* m_pGameCamera;
	float m_scale;
	DirectionalLight m_directionalLight;
	PersProjInfo m_persProjInfo;
};

#endif	/* GUI_H */