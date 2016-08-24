#pragma once

#ifndef CASCADED_SHADOW_MAP_TECHNIQUE_H
#define	CASCADED_SHADOW_MAP_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"

class CSMTechnique : public Technique {

public:

	CSMTechnique();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
	void SetTextureUnit(unsigned int TextureUnit);
private:

	GLuint m_WVPLocation;
	GLuint m_textureLocation;
};

#endif	/* CASCADED_SHADOW_MAP_TECHNIQUE_H */