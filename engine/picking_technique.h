#pragma once

#ifndef PICKING_TECHNIQUE_H
#define	PICKING_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"
#include "render_callbacks.h"
#include "ogldev_types.h"

class PickingTechnique : public Technique, public IRenderCallbacks
{
public:

    PickingTechnique();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);

    void SetObjectIndex(uint ObjectIndex);
    
    void DrawStartCB(uint DrawIndex);
    
private:
    
    GLuint m_WVPLocation;
    GLuint m_drawIndexLocation;
    GLuint m_objectIndexLocation;
};

#endif	/* PICKING_TECHNIQUE_H */