#pragma once

#ifndef OGLDEV_CALLBACKS_H
#define OGLDEV_CALLBACKS_H

#include "ogldev_keys.h"

class ICallbacks
{
public:

	virtual void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE OgldevKeyState = OGLDEV_KEY_STATE_PRESS) {};

	virtual void PassiveMouseCB(int x, int y) {};

	virtual void RenderSceneCB() {};

	virtual void MouseCB(OGLDEV_MOUSE Button, OGLDEV_KEY_STATE State, int x, int y) {};
};

#endif