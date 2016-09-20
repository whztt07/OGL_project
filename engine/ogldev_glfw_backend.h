#pragma once

#ifndef OGLDEV_GLFW_BACKEND_H
#define	OGLDEV_GLFW_BACKEND_H

#include "ogldev_types.h"
#include "ogldev_callbacks.h"

void GLFWBackendInit(int argc, char** argv, bool WithDepth, bool WithStencil);

void GLFWBackendTerminate();

bool GLFWBackendCreateWindow(uint Width, uint Height, bool isFullScreen, const char* pTitle);

void GLFWBackendRun(ICallbacks* pCallbacks);

void GLFWBackendSwapBuffers();

void GLFWBackendLeaveMainLoop();

void GLFWBackendSetMousePos(uint x, uint y);

#endif