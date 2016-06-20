/* 
	������ ��� �������� ����������� ������ �� OpenGL. �� �� ������ � �������� ��������, �� �������� ������ ������� �������� ������ � OpenGL.
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
#include "glew.h" /* 
						������������ ����������� run-time ��������� ��� ����������� ����, ����� OpenGL ���������� �������������� �� ������� ���������. 
						���� �� ����������� ������ ��������� OpenGL, ��� ������� ���������� ��� ������ ���������, ����� GLEW ��������� ��������. 
					*/
#include "freeglut.h" /* 
						API ��� ���������� ������� ��������, � ��� �� ��������� �������, �������� �����/������ � ��� ��������� ������ ������������ 
					*/
#include "math_3d.h" /* 
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
    Color = vec4(clamp(Position, 0.25, 1), 1.0);                                   \n\
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

	Matrix4f World;

	World.m[0][0] = sinf(Scale)*sinf(Scale);	World.m[0][1] = -sinf(Scale)*sinf(Scale);	World.m[0][2] = 0.0;						World.m[0][3] = 0.0;
	World.m[1][0] = sinf(Scale)*sinf(Scale);	World.m[1][1] = cosf(Scale)*sinf(Scale);	World.m[1][2] = 0.0;						World.m[1][3] = 0.0;
	World.m[2][0] = 0.0;						World.m[2][1] = 0.0;						World.m[2][2] = cosf(Scale)*sinf(Scale);	World.m[2][3] = 0.0;
	World.m[3][0] = 0.0;						World.m[3][1] = 0.0;						World.m[3][2] = 0.0;						World.m[3][3] = 1.0;

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]); // ������ � ������� �������� ����������� ������ �� ������

	glEnableVertexAttribArray(0); // ��������� ������������� �������� ������� ��� ������� � ���� ����� �������. ��� ���������� ��� ��������
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����������� ��������� ��� ��������� �����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); // ���� ����� ������� ��������� ��� ������������ ������ ������ ������.

	glDrawArrays(GL_QUADS, 0, 4);

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
	Vector3f Vertices[4];
	Vertices[0] = Vector3f(-0.75, -0.75, 0.0);
	Vertices[1] = Vector3f(0.75, -0.75, 0.0);
	Vertices[2] = Vector3f(0.75, 0.75, 0.0);
	Vertices[3] = Vector3f(-0.75, 0.75, 0.0);

	glGenBuffers(1, &VBO); // ������� ����� � ����� ����. ��� �������� ������ ������������ ��������� �������.
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ����������� ��������� ��� ���������� �������
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW); // ����� ���������� ������ �������, �� ��������� ��� �������.
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

	CompileShaders(); // ����������� �������

	glutMainLoop(); /*
						���� ����� ������� �������� GLUT��, ������� ������ ����� ���� ����������� ����. 
						� ���� ����� �� ��� ������� �� ������� ������� � ������� �� ����� ������� ��������� ������, ������� �� ������ �����.
					*/

	return 0;
}
