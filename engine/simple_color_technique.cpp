#include "simple_color_technique.h"
#include "util.h"

static const char* pVS = "                                                          \n\
#version 410                                                                        \n\
                                                                                    \n\
layout (location = 0) in vec3 Position;                                             \n\
                                                                                    \n\
uniform mat4 gWVP;                                                                  \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    gl_Position = gWVP * vec4(Position, 1.0);                                       \n\
}";

static const char* pFS = "                                                          \n\
#version 410                                                                        \n\
                                                                                    \n\
layout(location = 0) out vec4 FragColor;                                            \n\
                                                                                    \n\
void main()                                                                         \n\
{                                                                                   \n\
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);                                           \n\
}";

SimpleColorTechnique::SimpleColorTechnique()
{
}

bool SimpleColorTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, pVS)) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, pFS)) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_WVPLocation = GetUniformLocation("gWVP");

	if (m_WVPLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void SimpleColorTechnique::SetWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}