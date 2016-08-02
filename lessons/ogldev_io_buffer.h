#pragma once

#ifndef IO_BUFFER_H
#define	IO_BUFFER_H

#include <GL/glew.h>

class IOBuffer
{
public:
    
    IOBuffer();

    ~IOBuffer();

    bool Init(uint WindowWidth, uint WindowHeight, bool WidthDepth, GLenum InternalType);

    void BindForWriting();
   
    void BindForReading(GLenum TextureUnit);          

private:
                     
    GLuint m_fbo;
    GLuint m_texture;
    GLuint m_depth;
    GLenum m_internalType;
};

#endif	/* IO+BUFFER_H */