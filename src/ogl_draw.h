#pragma once

#ifndef OGL_DRAW_H
#define	OGL_DRAW_H

#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "cell.h"

using namespace std;

static void RenderSceneCB();

static void InitializeGlutCallbacks();

static void CreateVertexBuffer();

static void CreateIndexBuffer(vector<Cell> lab);

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

static void CompileShaders();

void Draw(int argc, char** argv, vector<Cell> lab, int size);

#endif	/* GUI_H */