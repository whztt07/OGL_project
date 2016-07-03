/* 
	Данный код является результатом уроков по OpenGL и помогает понять базовые принципы работы с ним.
	Оригинальный сайт с уроками: http://ogldev.atspace.co.uk/index.html
	Переводы: http://triplepointfive.github.io/ogltutor/
*/

#include <math.h>
#include <GL/glew.h> /* 
						Обеспечивает эффективные run-time механизмы для определения того, какие OpenGL расширения поддерживаются на целевой платформе. 
						Если вы подключаете другие заголовки OpenGL, вам следует подключить его раньше остальных, иначе GLEW откажется работать. 
					*/
#include <GL/freeglut.h> /* 
						API для управления оконной системой, а так же обработка событий, контроль ввода/вывода и ещё несколько других возможностей 
					*/

#include "pipeline.h" /* 
						Кастомный заголовок, постепенно увеличивающийся в ходе обучения.
						Он содержит фиксированный конвеер функций, необходимых для работы с графическими объектами.
					*/
#include "camera.h" /*
						Это объявление заголовочника класса камеры. 
						Он хранит 3 свойства, которые характеризуют камеру - позиция, направление и верхний вектор. 
						Так же добавлены 2 конструктора. 
						По умолчанию просто располагает камеру в начале координат, 
						направляет ее в сторону уменьшения Z, а верхний вектор устремляет в “небо” (0,1,0). 
						Но есть возможность создать камеру с указанием значений атрибутов.
					*/
#include "texture.h"
#include "lighting_technique.h"
#include "glut_backend.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct Vertex
{
	Vector3f m_pos;
	Vector2f m_tex;

	Vertex() {}

	Vertex(Vector3f pos, Vector2f tex)
	{
		m_pos = pos;
		m_tex = tex;
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
		m_directionalLight.AmbientIntensity = 0.5f;
	}

	~Main()
	{
		delete m_pEffect;
		delete m_pGameCamera;
		delete m_pTexture;
	}

	bool Init()
	{
		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

		CreateVertexBuffer();
		CreateIndexBuffer();

		m_pEffect = new LightingTechnique();

		if (!m_pEffect->Init())
		{
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
		p.WorldPos(0.0f, 0.0f, 3.0f);
		p.SetCamera(m_pGameCamera->GetPos(), m_pGameCamera->GetTarget(), m_pGameCamera->GetUp());
		p.SetPerspectiveProj(60.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);
		m_pEffect->SetWVP(p.GetTrans());
		m_pEffect->SetDirectionalLight(m_directionalLight);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		m_pTexture->Bind(GL_TEXTURE0);
		glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

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
		}
	}


	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}

private:

	void CreateVertexBuffer()
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

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	}

	void CreateIndexBuffer()
	{
		unsigned int Indices[] = { 0, 1, 5, 4,
			3, 0, 4, 7,
			2, 3, 7, 6,
			1, 2, 6, 5,
			3, 2, 1, 0,
			4, 5, 6, 7 };

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
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

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "OpenGL tutors")) {
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