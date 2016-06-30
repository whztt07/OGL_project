/* 
	������ ��� �������� ����������� ������ �� OpenGL, � �� �������� ������ ������� �������� ������ � OpenGL.
	��� ��� ��� �������� �� ������� ����� ��������� ��������, �� ��������� ������������� ������ �������� �����������, ������ �� "��� � ��� ����". �����.
	� ����� ����������� ������������ ���, ��� ��� ����� ����� � "��� �������� �� �����������". 
	� �����������.
	������������ ���� � �������: http://ogldev.atspace.co.uk/index.html
	��������: http://triplepointfive.github.io/ogltutor/
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <GL/glew.h> /* 
						������������ ����������� run-time ��������� ��� ����������� ����, ����� OpenGL ���������� �������������� �� ������� ���������. 
						���� �� ����������� ������ ��������� OpenGL, ��� ������� ���������� ��� ������ ���������, ����� GLEW ��������� ��������. 
					*/
#include <GL/freeglut.h> /* 
						API ��� ���������� ������� ��������, � ��� �� ��������� �������, �������� �����/������ � ��� ��������� ������ ������������ 
					*/

#include "pipeline.h" /* 
						��������� ���������, ���������� ��������������� � ���� ��������.
						�� �������� ������������� ������� �������, ����������� ��� ������ � ������������ ���������.
					*/
#include "camera.h" /*
						��� ���������� ������������� ������ ������. 
						�� ������ 3 ��������, ������� ������������� ������ - �������, ����������� � ������� ������. 
						��� �� ��������� 2 ������������. 
						�� ��������� ������ ����������� ������ � ������ ���������, 
						���������� �� � ������� ���������� Z, � ������� ������ ���������� � ����� (0,1,0). 
						�� ���� ����������� ������� ������ � ��������� �������� ���������.
					*/
#include "texture.h"

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

GLuint VBO; // ���������� ���������� ��� �������� ��������� �� ����� ������
GLuint IBO; // �� �� ����� ��� ������ �������� ���� ������
GLuint gWVPLocation; /*
							�� ���������� ���� ��������� ��� ������� � ��������� �������, 
							�������������� � ���� uniform-���������� ������ �������. 
							��������� ��� ������, ��� �� ��� �� ������ � ��������, ��� ��������� ��� ������� � �����, 
							������� �� ��������� ������������ ������������ ������� ������ ������ ������������ ������.
					   */
GLuint gSampler;
Texture* pTexture = NULL;
Camera* pGameCamera = NULL;

/*
	��-�� ����������� ���������� ������������� ��� �������� ���������������� �� �������.
	��� �� �����, ����� ���������� �� ���������� ������� �������� (���), ��� ��� �� ��� ��� �����.
	����� =(
*/
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
layout (location = 1) in vec2 TexCoord;                                             \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
out vec2 TexCoord0;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
    TexCoord0 = TexCoord;                                                           \n\
}"; /*��������� ������*/

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec2 TexCoord0;                                                                  \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
uniform sampler2D gSampler;                                                         \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = texture2D(gSampler, TexCoord0.xy);                                  \n\
}"; /*���������� ������*/


static void RenderSceneCB()
{
	pGameCamera->OnRender();

	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;

	Scale += 0.1f;

    Pipeline p;
	p.Rotate(0.0f, 0.0f, 0.0f);
    p.WorldPos(0.0f, 0.0f, 3.0f);
	p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
	p.SetPerspectiveProj(60.0, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 100.0f);

	glUniformMatrix4fv(gWVPLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans()); // ������ � ������� �������� ����������� ������ �� ������

	glEnableVertexAttribArray(0); // ��������� ������������� �������� ������� ��� ������� � ���� ����� �������. ��� ���������� ��� ��������
	glEnableVertexAttribArray(1); // ��������� ������������� ��������� ������ 1 ��� ��������� ������� � ���������� � �������� 0, ������� ��� ����� ��� �������.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����������� ��������� �� ������� ��� ��������� �����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // ���� ����� ������� ��������� ��� ������������ ������ ������ ������ �����.
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12); // ���� ����� ��� ������ �������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // ����������� ��������� �� ������� ��� ��������� �����
	pTexture->Bind(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0); /*
										��� ������� �������� ���� ��������� ������ ������� �������, ��� ������ �������� ������������� � ���. 
										�������� ��� ����������, ���� ������ �� ������������ - ������ ������ �������� �������.
								   */
	glDisableVertexAttribArray(1);

	glutSwapBuffers(); /*
							����� ��������� ����� ������� OpenGL ������� �������� ������ ��� ������ glutSwapBuffers (), ���� � ��� �������� ������� �����������. 
							��� �������� �� ������� �� ������������ ������ � ����� ���������� ����� �������. 
							�������� ��� ��� ��������� ������� �������� � ��� ����, ����� �� ���� ������� �������� ������.
					   */
}


static void SpecialKeyboardCB(int Key, int x, int y)
{
	pGameCamera->OnKeyboard(Key);
} // ������� ���������� ������� � ������� ������� � ������ ������� ������.

static void KeyboardCB(unsigned char Key, int x, int y)
{
	switch (Key) {
	case 27 : // 27 == ESC
		glutLeaveMainLoop();
	}
}

static void PassiveMouseCB(int x, int y)
{
	pGameCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB); // ��������� ������� ������� � �������� "�������"
	glutSpecialFunc(SpecialKeyboardCB); /*
											��� ������� ���������� ���� ��� ������, ���� ����� ������ ������������� �������. 
											� ������ ����������� ������ ������: F{1..12}, ������� � PAGE-UP / PAGE-DOWN / HOME / END / INSERT. 
											���� ��� ����� ������� ������ (������� � �����) ����������� glutKeyboardFunc().
										*/
	glutPassiveMotionFunc(PassiveMouseCB); // ��������� �������� ��������, ��� ���� �������� ��� ������� �����-���� ������.
	glutKeyboardFunc(KeyboardCB);
}

static void CreateVertexBuffer()
{
	Vertex Vertices[8];
	Vertices[0] = Vertex(Vector3f(0.5, 0.5, -0.5), Vector2f(0.0, 0.0));
	Vertices[1] = Vertex(Vector3f(0.5, -0.5, -0.5), Vector2f(1.0, 0.0));
	Vertices[2] = Vertex(Vector3f(-0.5, -0.5, -0.5), Vector2f(1.0, 1.0));
	Vertices[3] = Vertex(Vector3f(-0.5, 0.5, -0.5), Vector2f(0.0, 1.0));
	Vertices[4] = Vertex(Vector3f(0.5, 0.5, 0.5), Vector2f(1.0, 0.0));
	Vertices[5] = Vertex(Vector3f(0.5, -0.5, 0.5), Vector2f(1.0, 1.0));
	Vertices[6] = Vertex(Vector3f(-0.5, -0.5, 0.5), Vector2f(0.0, 1.0));
	Vertices[7] = Vertex(Vector3f(-0.5, 0.5, 0.5), Vector2f(0.0, 0.0));

	glGenBuffers(1, &VBO); // ������� ����� � ����� ����. ��� �������� ������ ������������ ��������� �������.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����������� ��������� ��� ���������� �������
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // ����� ���������� ������ �������, �� ��������� ��� �������.
}

static void CreateIndexBuffer()
{
	unsigned int Indices[] = {	0, 1, 2,
								2, 3, 0,
								0, 4, 3,
								3, 7, 4,
								4, 0, 1,
								1, 4, 5,
								5, 1, 2,
								2, 5, 6,
								6, 2, 3,
								3, 6, 7,
								7, 4, 5,
								5, 6, 7 };

	glGenBuffers(1, &IBO); // ����� � ����: �� ���������� CreateVertexBuffer()
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(1);
	}

	const GLchar* shdrTxt[1];
	shdrTxt[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, shdrTxt, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj); // ������������ ���������������� ������ ������� � ������� ���������
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram(); /*
													�� �������� ������� ���������� �������� ����� �������� ������������ �������. 
													����� �� �������� ��� ������� � ���� ������.
											  */

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER); // �������� ���������� �������
	AddShader(ShaderProgram, pFS, GL_FRAGMENT_SHADER); // �������� ������������ (�����������) �������

	GLint success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if (success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram); /*
										��� ������������� ������������� ��������� �������� �� ��������� � ��� ��������� ��������� ����� ����. 
										��� ��������� �������� ������ ��� ���� ������� ���������, ���� �� �� �������� � ������ 
										��� �� ��������� � ������������� �������� �������� glUseProgram � ���������� NULL. 
										���� �� ��������� ��������� ���������, ���������� ������ 1 ��� ��������, 
										����� ������ ����� ����� ������������ ���� ���������������� ��-���������.
								 */

	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP"); // ����������� ������� uniform-���������� gWP � ����������� �������
	assert(gWVPLocation != 0xFFFFFFFF); /*
												����� ����� �������� �� ������ (��� �� � ������� ���), ����� ���������� ���������� �� ������� � ������. 
												���� 2 �������� ������� ������ � ���� �������. 
												�� �������� � ������� ��� ���������� ��� ��� ���� ������ ������������ � ����� �����������. 
												���� ���������� �� ��������� ������������� ����������, �� ��� �������� �������� �. 
												� ���� ������ glGetUniformLocation �� ���� ����������.
										  */
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler"); // ���������� � gSampler
	assert(gSampler != 0xFFFFFFFF);
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv); // �������������� GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*
														��� ������������� ��������� ����� GLUT. 
														GLUT_DOUBLE �������� ������� ����������� (��������� ����� ����������� � ������� �����, � �� ����� ��� ������ ����� ������������) 
														� ����� �����, ���������� ������������� ��� �������� ���� ���������� (�.�. �� ������).
												  */
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial XX"); // ����� �������� ��������� ����. � ������������ ������ � ������� ��������� ���� �������������� ���������� �����. ����� �� ������� �� ��.
	glutGameModeString("800x600@32"); // ��� ������� glut�� ��������� ������ ���������� ����������� � ������������� ������, ���������� ��� �������� �����. 
	//glutEnterGameMode();

	InitializeGlutCallbacks(); // ������� ����� �������������� � �������� ���������� ����� ������� ��������� ������

	pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	/* ������ �� �������������� GLEW � ��������� �� ������. GLUT ���������� ���������������� ������. */
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(err));
		return 1;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // ��������� "�������" ����� ����
	//glFrontFace(GL_CW);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);

	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShaders(); // ����������� �������

	glUniform1i(gSampler, 0);

	pTexture = new Texture(GL_TEXTURE_2D, "../thirdparty/content/test.png");
	//pTexture = new Texture(GL_TEXTURE_2D, "thirdparty/content/test.png");

	if (!pTexture->Load()) {
		return 1;
	}

	glutMainLoop(); /*
						���� ����� ������� �������� GLUT��, ������� ������ ����� ���� ����������� ����. 
						� ���� ����� �� ��� ������� �� ������� ������� � ������� �� ����� ������� ��������� ������, ������� �� ������ �����.
					*/

	system("Pause");
	return 0;
}
