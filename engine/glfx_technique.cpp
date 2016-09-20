#include <stdio.h>
#include <string>

#include <glfx.h>

#include "glfx_technique.h"
#include "ogldev_util.h"

using namespace std;

GLFXTechnique::GLFXTechnique(const char* pEffectFile)
{
    m_pEffectFile = pEffectFile;
    m_shaderProg = 0;
    m_effect = glfxGenEffect();
}

GLFXTechnique::~GLFXTechnique()
{
    if (m_shaderProg != 0)
    {
        glDeleteProgram(m_shaderProg);
        m_shaderProg = 0;
    }
    
    glfxDeleteEffect(m_effect);
}

bool GLFXTechnique::CompileProgram(const char* pProgram)
{
    if (!glfxParseEffectFromFile(m_effect, m_pEffectFile)) {
        string log = glfxGetEffectLog(m_effect);
        printf("Error creating effect from file '%s':\n", m_pEffectFile);
        printf("%s\n", log.c_str());
        return false;
    }
    
    m_shaderProg = glfxCompileProgram(m_effect, pProgram);
    
    if (m_shaderProg < 0) {
        string log = glfxGetEffectLog(m_effect);
        printf("Error compiling program '%s' in effect file '%s':\n", pProgram, m_pEffectFile);
        printf("%s\n", log.c_str());
        return false;
    }
    
    return true;
}

void GLFXTechnique::Enable()
{
    glUseProgram(m_shaderProg);
}

GLint GLFXTechnique::GetUniformLocation(const char* pUniformName)
{
    GLuint Location = glGetUniformLocation(m_shaderProg, pUniformName);

    if (Location == INVALID_OGL_VALUE) {
        fprintf(stderr, "Warning! Unable to get the location of uniform '%s'\n", pUniformName);
    }

    return Location;
}

GLint GLFXTechnique::GetProgramParam(GLint param)
{
    GLint ret;
    glGetProgramiv(m_shaderProg, param, &ret);
    return ret;
}