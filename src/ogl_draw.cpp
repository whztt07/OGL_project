#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>

#include "ogl_draw.h"

#include "ogldev_util.h"
#include "ogldev_pipeline.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024

static GLuint VBO;
static GLuint IBO;
static GLuint gWorldLocation;
static PersProjInfo gPersProjInfo;
static unsigned int MazeSize;
static unsigned int IndicesCount;

static const char* pVSFileName = "engine/shaders/shader.vs";
static const char* pFSFileName = "engine/shaders/shader.fs";

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	static float Scale = 0.0f;

	Scale += 0.1f;

	Pipeline p;

	p.WorldPos(0.0f, 0.0f, 5.0f);
	p.SetPerspectiveProj(gPersProjInfo);

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetWPTrans());

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	glDrawElements(GL_POINTS, IndicesCount, GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
	glutIdleFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{
	float bit = (float) 2 / MazeSize;
	std::vector<Vector3f> Vertices;
	for (int y = 0; y <= MazeSize; y++)
		for (int x = 0; x <= MazeSize; x++) {
			float cellx = bit*x - 1;
			float celly = 1 - bit*y;
			float cellz = 0;
			Vertices.push_back(Vector3f(cellx, celly, cellz));
		}

	/*for (int i = 0; i < Vertices.size(); i++)
		printf("(%2.1f %2.1f %2.1f) ", Vertices[i].x, Vertices[i].y, Vertices[i].z);
	system("PAUSE");*/

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vector3f), &Vertices[0], GL_STATIC_DRAW);
}

static void CreateIndexBuffer(vector<Cell> lab)
{
	std::vector<unsigned int> Indices(1);

	for (int i = 0; i < MazeSize*MazeSize; i++) {
		//printf("[%d][%d]: (u%d d%d l%d r%d)\n", y, x, lab[y*MazeSize + x].Top, lab[y*MazeSize + x].Bottom, lab[y*MazeSize + x].Left, lab[y*MazeSize + x].Right);
		/*if (lab[i].Top == Close) {
			Indices.push_back(i);
			Indices.push_back(i + 1);
			printf("Zakryvaem verh\n");
		}*/
		/*if (lab[i].Bottom == Close) {
			Indices.push_back(i + MazeSize);
			Indices.push_back(i + MazeSize + 1);
			printf("Zakryvaem niz\n");
		}*/
		/*if (lab[i].Left == Close) {
			Indices.push_back(i);
			Indices.push_back(i + MazeSize);
			printf("Zakryvaem levo\n");
		}*/
		/*if (lab[i].Right == Close) {
			Indices.push_back(i + 1);
			Indices.push_back(i + MazeSize + 1);
			printf("Zakryvaem prav\n");
		}*/
	}

	for (int i = 0; i < Indices.size(); i++)
		printf("%4d",Indices[i]);

	IndicesCount = Indices.size();

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);
}

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
		exit(1);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		exit(1);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

static void CompileShaders()
{
	GLuint ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
		exit(1);
	}

	string vs, fs;

	if (!ReadFile(pVSFileName, vs)) {
		exit(1);
	};

	if (!ReadFile(pFSFileName, fs)) {
		exit(1);
	};

	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
		exit(1);
	}

	glUseProgram(ShaderProgram);

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	assert(gWorldLocation != 0xFFFFFFFF);
}

void Draw(int argc, char** argv, vector<Cell> lab, int size)
{
	MazeSize = size+1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Labyrinth");

	InitializeGlutCallbacks();

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();
	CreateIndexBuffer(lab);

	CompileShaders();

	gPersProjInfo.FOV = 30.0f;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.zNear = 0.0f;
	gPersProjInfo.zFar = 100.0f;

	glutMainLoop();

	return;
}