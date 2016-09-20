#pragma once

#ifndef SHADOW_VOLUME_TECHNIQUE_H
#define	SHADOW_VOLUME_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"

class ShadowVolumeTechnique : public Technique {
public:

    ShadowVolumeTechnique();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);
    void SetLightPos(const Vector3f& Pos);
    
private:
    
    GLuint m_WVPLocation;
    GLuint m_lightPosLocation;
};

#endif	/* SHADOW_VOLUME_TECHNIQUE_H */