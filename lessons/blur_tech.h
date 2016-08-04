#pragma once

#ifndef BLUR_TECH_H
#define	BLUR_TECH_H

#include "technique.h"
#include "ogldev_math_3d.h"
#include "ogldev_io_buffer.h"

class BlurTech : public Technique {
public:

	BlurTech();

	virtual bool Init();

	void BindInputBuffer(IOBuffer& inputBuf);

private:

	GLuint m_inputTextureUnitLocation;
};

#endif