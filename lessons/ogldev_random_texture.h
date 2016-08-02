#pragma once

#ifndef OGLDEV_RANDOM_TEXTURE_H
#define OGLDEV_RANDOM_TEXTURE_H

#include <GL/glew.h>

#include "ogldev_types.h"

class RandomTexture
{
public:
    RandomTexture();
    
    bool Init(uint size);
    
    ~RandomTexture();
    
    void Bind(GLenum TextureUnit);
    
private:
    uint m_texture;
};

#endif