/* 
	������ ��� �������� ����������� ������ �� OpenGL. �� �� ������ � �������� ��������, �� �������� ������ ������� �������� ������ � OpenGL.
	��� ��� ��� �������� �� ������� ����� ��������� ��������, �� ��������� ������������� ������ �������� �����������, ������ �� "��� � ��� ����". �����.
	� ����� ����������� ������������ ���, ��� ��� ����� ����� � "��� �������� �� �����������". 
	� �����������.
	������������ ���� � �������: http://ogldev.atspace.co.uk/index.html
	��������: http://triplepointfive.github.io/ogltutor/
*/

#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include "glew.h" /* 
						������������ ����������� run-time ��������� ��� ����������� ����, ����� OpenGL ���������� �������������� �� ������� ���������. 
						���� �� ����������� ������ ��������� OpenGL, ��� ������� ���������� ��� ������ ���������, ����� GLEW ��������� ��������. 
					*/
#include "freeglut.h" /* 
						API ��� ���������� ������� ��������, � ��� �� ��������� �������, �������� �����/������ � ��� ��������� ������ ������������ 
					*/

#include "pipeline.h" /* 
						��������� ���������, ���������� ��������������� � ���� �������� 
					*/

#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__ " with") /*
						������ "Compiled on ����-���������� at �����-����������" ����� �������� � ��� ����
						�� �� ��� ������ �� �����, �� ����� ����� � �������� � ���� ������.
					*/
#pragma comment( compiler ) /*
						� ������� ����� �������� ��� � ������ �����������. 
						�� �� ��� �� ������, �� ����� ����� ����� ����������.
					*/
					
GLuint VBO; // ���������� ���������� ��� �������� ��������� �� ����� ������
GLuint IBO; // �� �� ����� ��� ������ �������� ���� ������
GLuint gWorldLocation; /*
							�� ���������� ���� ��������� ��� ������� � ��������� �������, 
							�������������� � ���� uniform-���������� ������ �������. 
							��������� ��� ������, ��� �� ��� �� ������ � ��������, ��� ��������� ��� ������� � �����, 
							������� �� ��������� ������������ ������������ ������� ������ ������ ������������ ������.
					   */

/*
	��-�� ����������� ���������� ������������� ��� �������� ���������������� �� �������.
	��� �� �����, ����� ���������� �� ���������� ������� �������� (���), ��� ��� �� ��� ��� �����.
	����� =(
*/
static const char* pVS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWorld;                                                                \n\
                                                                                    \n\
out vec4 Color;                                                                     \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWorld * vec4(Position, 1.0);                                     \n\
    Color = vec4(clamp(Position, 0.0, 1.0), 1.0);                                   \n\
}"; /*��������� ������*/

static const char* pFS = "                                                          \n\
#version 330                                                                        \n\
                                                                                    \n\
in vec4 Color;                                                                      \n\
                                                                                    \n\
out vec4 FragColor;                                                                 \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = Color;                                                              \n\
}"; /*���������� ������*/


static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0;

	Scale += 0.01;

    Pipeline p;
    p.Scale(sinf(Scale * 0.1), sinf(Scale * 0.1), sinf(Scale * 0.1));
    p.WorldPos(sinf(Scale), 0.0, 0.0);
    p.Rotate(sinf(Scale) * 90.0, sinf(Scale) * 90.0, sinf(Scale) * 90.0);

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans()); // ������ � ������� �������� ����������� ������ �� ������

	glEnableVertexAttribArray(0); // ��������� ������������� �������� ������� ��� ������� � ���� ����� �������. ��� ���������� ��� ��������
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����������� ��������� �� ������� ��� ��������� �����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // ���� ����� ������� ��������� ��� ������������ ������ ������ ������.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // ����������� ��������� �� ������� ��� ��������� �����

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0); /*
										��� ������� �������� ���� ��������� ������ ������� �������, ��� ������ �������� ������������� � ���. 
										�������� ��� ����������, ���� ������ �� ������������ - ������ ������ �������� �������.
								   */

	glutSwapBuffers(); /*
							����� ��������� ����� ������� OpenGL ������� �������� ������ ��� ������ glutSwapBuffers (), ���� � ��� �������� ������� �����������. 
							��� �������� �� ������� �� ������������ ������ � ����� ���������� ����� �������. 
							�������� ��� ��� ��������� ������� �������� � ��� ����, ����� �� ���� ������� �������� ������.
					   */
}


static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB); // ��������� ������� ������� � �������� "�������"
}

static void CreateVertexBuffer()
{
	Vector3f Vertices[8];
	Vertices[0] = Vector3f(0.5, -0.5, -0.5);
	Vertices[1] = Vector3f(-0.5, -0.5, -0.5);
	Vertices[2] = Vector3f(-0.5, 0.5, -0.5);
	Vertices[3] = Vector3f(0.5, 0.5, -0.5);
	Vertices[4] = Vector3f(0.5, -0.5, 0.5);
	Vertices[5] = Vector3f(-0.5, -0.5, 0.5);
	Vertices[6] = Vector3f(-0.5, 0.5, 0.5);
	Vertices[7] = Vector3f(0.5, 0.5, 0.5);

	glGenBuffers(1, &VBO); // ������� ����� � ����� ����. ��� �������� ������ ������������ ��������� �������.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����������� ��������� ��� ���������� �������
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // ����� ���������� ������ �������, �� ��������� ��� �������.
}

static void CreateIndexBuffer()
{
	unsigned int Indices[] = {  0, 1, 2,
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
		exit(0);
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

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld"); // ����������� ������� uniform-���������� � ����������� �������
	assert(gWorldLocation != 0xFFFFFFFF); /*
												����� ����� �������� �� ������ (��� �� � ������� ���), ����� ���������� ���������� �� ������� � ������. 
												���� 2 �������� ������� ������ � ���� �������. 
												�� �������� � ������� ��� ���������� ��� ��� ���� ������ ������������ � ����� �����������. 
												���� ���������� �� ��������� ������������� ����������, �� ��� �������� �������� �. 
												� ���� ������ glGetUniformLocation �� ���� ����������.
										  */
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv); // �������������� GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); /*
														��� ������������� ��������� ����� GLUT. 
														GLUT_DOUBLE �������� ������� ����������� (��������� ����� ����������� � ������� �����, � �� ����� ��� ������ ����� ������������) 
														� ����� �����, ���������� ������������� ��� �������� ���� ���������� (�.�. �� ������).
												  */
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial XX"); // ����� �������� ��������� ����. � ������������ ������ � ������� ��������� ���� �������������� ���������� �����. ����� �� ������� �� ��.

	InitializeGlutCallbacks(); // ������� ����� �������������� � �������� ���������� ����� ������� ��������� ������

	/* ������ �� �������������� GLEW � ��������� �� ������. GLUT ���������� ���������������� ������. */
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(err));
		return 1;
	}

	glClearColor(0.0, 0.0, 0.0, 0.0); // ��������� "�������" ����� ����

	CreateVertexBuffer();
	CreateIndexBuffer();

	CompileShaders(); // ����������� �������

	glutMainLoop(); /*
						���� ����� ������� �������� GLUT��, ������� ������ ����� ���� ����������� ����. 
						� ���� ����� �� ��� ������� �� ������� ������� � ������� �� ����� ������� ��������� ������, ������� �� ������ �����.
					*/

	return 0;
}
