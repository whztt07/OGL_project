#pragma once

#ifndef DS_GEOM_PASS_TECH_H
#define	DS_GEOM_PASS_TECH_H

#include "technique.h"
#include "ogldev_math_3d.h"

class DSGeomPassTech : public Technique {
public:

    DSGeomPassTech();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);
    void SetWorldMatrix(const Matrix4f& WVP);
	void SetColorTextureUnit(unsigned int TextureUnit);

private:

    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
	GLuint m_colorTextureUnitLocation;
};

#endif