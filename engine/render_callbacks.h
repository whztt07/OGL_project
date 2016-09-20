#pragma once

#ifndef RENDER_CALLBACKS_H
#define	RENDER_CALLBACKS_H

class IRenderCallbacks
{
public:
	virtual void DrawStartCB(unsigned int DrawIndex) = 0;
};

#endif	/* RENDER_CALLBACKS_H */