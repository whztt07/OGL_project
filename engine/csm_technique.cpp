#include "ogldev_util.h"
#include "csm_technique.h"

CSMTechnique::CSMTechnique()
{
}

bool CSMTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "engine/shaders/csm.vs")) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "engine/shaders/csm.fs")) {
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

void CSMTechnique::SetWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}