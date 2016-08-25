#pragma once

#ifndef OGLDEV_KEYS_H
#define OGLDEV_KEYS_H

#include "ogldev_types.h"
#include "ogldev_callbacks.h"

enum OGLDEV_BACKEND_TYPE {
    OGLDEV_BACKEND_TYPE_GLUT,
    OGLDEV_BACKEND_TYPE_GLFW
};

void OgldevBackendInit(OGLDEV_BACKEND_TYPE BackendType, int argc, char** argv, bool WithDepth, bool WithStencil);

void OgldevBackendTerminate();

bool OgldevBackendCreateWindow(uint Width, uint Height, bool isFullScreen, const char* pTitle);

void OgldevBackendRun(ICallbacks* pCallbacks);

void OgldevBackendLeaveMainLoop();

void OgldevBackendSwapBuffers();

void OgldevBackendSetMousePos(uint x, uint y);

#endif