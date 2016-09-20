#pragma once

#ifndef DS_DIR_LIGHT_PASS_TECH_H
#define	DS_DIR_LIGHT_PASS_TECH_H

#include "ds_light_pass_tech.h"
#include "ogldev_lights_common.h"

class DSDirLightPassTech : public DSLightPassTech {
public:

    DSDirLightPassTech();
    
    virtual bool Init();

    void SetDirectionalLight(const DirectionalLight& Light);

private:

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint DiffuseIntensity;
        GLuint Direction;
    } m_dirLightLocation;
};

#endif