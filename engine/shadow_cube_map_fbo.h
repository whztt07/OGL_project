#pragma once

#ifndef SHADOW_CUBE_MAP_FBO_H
#define	SHADOW_CUBE_MAP_FBO_H

#include <GL/glew.h>

class ShadowCubeMapFBO
{
public:
    ShadowCubeMapFBO();

    ~ShadowCubeMapFBO();

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting(GLenum CubeFace);

    void BindForReading(GLenum TextureUnit);
    
private:
    GLuint m_fbo;
    GLuint m_shadowMap;
    GLuint m_depth;
};

#endif	/* SHADOW_CUBE_MAP_FBO_H */