#pragma once

#ifndef OGL_DRAW_H
#define	OGL_DRAW_H

#include <GL/glew.h>
#include <GL/freeglut.h>

static void RenderSceneCB();

static void InitializeGlutCallbacks();

static void CreateVertexBuffer();

static void CreateIndexBuffer();

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

static void CompileShaders();

void Draw(int argc, char** argv);

#endif	/* GUI_H */