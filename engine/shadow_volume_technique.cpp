#include <limits.h>
#include <string>

#include "shadow_volume_technique.h"

using namespace std;

ShadowVolumeTechnique::ShadowVolumeTechnique()
{   
}

bool ShadowVolumeTechnique::Init()
{
    if (!Technique::Init()) {
        return false;
    }

    if (!AddShader(GL_VERTEX_SHADER, "engine/shaders/shadow_volume.vs")) {
        return false;
    }

    if (!AddShader(GL_GEOMETRY_SHADER, "engine/shaders/shadow_volume.gs")) {
        return false;
    }

    if (!AddShader(GL_FRAGMENT_SHADER, "engine/shaders/shadow_volume.fs")) {
        return false;
    }

    if (!Finalize()) {
        return false;
    }
    
    m_WVPLocation = GetUniformLocation("gWVP");
    m_lightPosLocation = GetUniformLocation("gLightPos");

    if (m_WVPLocation == INVALID_UNIFORM_LOCATION ||
        m_lightPosLocation == INVALID_UNIFORM_LOCATION) {
        return false;
    }
            
    return true;
}

void ShadowVolumeTechnique::SetWVP(const Matrix4f& WVP)
{
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
}

void ShadowVolumeTechnique::SetLightPos(const Vector3f& Pos)
{
    glUniform3f(m_lightPosLocation, Pos.x, Pos.y, Pos.z);
}