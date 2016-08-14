#pragma once

#ifndef GLFX_TECHNIQUE_H
#define	GLFX_TECHNIQUE_H

#include <list>
#include <GL/glew.h>

class GLFXTechnique
{
public:

    GLFXTechnique(const char* pEffectFile);

    ~GLFXTechnique();

    void Enable();

protected:
    
    bool CompileProgram(const char* pProgram);
    
    GLint GetUniformLocation(const char* pUniformName);
    
    GLint GetProgramParam(GLint param);

private:    
    GLint m_effect;    
    GLint m_shaderProg;
    const char* m_pEffectFile;
};

#endif	/* GLFX_TECHNIQUE_H */