/* 
	������ ��� �������� ����������� ������ �� OpenGL � �������� ������ ������� �������� ������ � ���.
	������������ ���� � �������: http://ogldev.atspace.co.uk/index.html
	��������: http://triplepointfive.github.io/ogltutor/
*/

#include <math.h>
#include <GL/glew.h> /* 
						������������ ����������� run-time ��������� ��� ����������� ����, ����� OpenGL ���������� �������������� �� ������� ���������. 
						���� �� ����������� ������ ��������� OpenGL, ��� ������� ���������� ��� ������ ���������, ����� GLEW ��������� ��������. 
					*/
#include <GL/freeglut.h> /* 
						API ��� ���������� ������� ��������, � ��� �� ��������� �������, �������� �����/������ � ��� ��������� ������ ������������ 
					*/

#include "pipeline.h"
#include "camera.h"
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"
#include "util.h"

#define WINDOW_WIDTH 800
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

class Main : public ICallbacks
{
public:

	Main()
	{
		m_pGameCamera = NULL;
		m_pTexture = NULL;
		m_pEffect = NULL;
		m_scale = 0.0f;
		m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_directionalLight.AmbientIntensity = 0.0f;
		m_directionalLight.DiffuseIntensity = 0.75f;
		m_directionalLight.Direction = Vector3f(1.0f, 0.0, 0.0);
	}

	~Main()
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

		unsigned int Indices[] = {	0, 1, 5,
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

		m_pEffect->SetTextureUnit(0);

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

		m_scale += 0.3f;

		Pipeline p;
		p.Rotate(m_scale, m_scale, m_scale);
		p.WorldPos(0.0f, 0.0f, 1.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
		m_pEffect->SetWVP(p.GetWVPTrans());
		const Matrix4f& WorldTransformation = p.GetWorldTrans();
		m_pEffect->SetWorldMatrix(WorldTransformation);
		m_pEffect->SetDirectionalLight(m_directionalLight);

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

	virtual void SpecialKeyboardCB(int Key, int x, int y)
	{
		m_pGameCamera->OnKeyboard(Key);
	}


	virtual void KeyboardCB(unsigned char Key, int x, int y)
	{
		switch (Key) {
		case 27: // 27 == ESC
			glutLeaveMainLoop();
			break;

		case 'a':
			m_directionalLight.AmbientIntensity += 0.05f;
			break;

		case 's':
			m_directionalLight.AmbientIntensity -= 0.05f;
			break;

		case 'z':
			m_directionalLight.DiffuseIntensity += 0.05f;
			break;

		case 'x':
			m_directionalLight.DiffuseIntensity -= 0.05f;
			break;
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
	DirectionLight m_directionalLight;
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