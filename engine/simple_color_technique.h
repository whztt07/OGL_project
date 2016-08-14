#pragma once

#ifndef SIMPLE_COLOR_TECHNIQUE_H
#define	SIMPLE_COLOR_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"

class SimpleColorTechnique : public Technique
{
public:

    SimpleColorTechnique();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);

private:
    
    GLuint m_WVPLocation;
};

#endif	/* SIMPLE_COLOR_TECHNIQUE_H */