#pragma once

#ifndef CASCADED_SHADOW_MAP_TECHNIQUE_H
#define	CASCADED_SHADOW_MAP_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"

class ShadowMapTechnique : public Technique {

public:

	ShadowMapTechnique();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
private:

	GLuint m_WVPLocation;
};

#endif	/* CASCADED_SHADOW_MAP_TECHNIQUE_H */