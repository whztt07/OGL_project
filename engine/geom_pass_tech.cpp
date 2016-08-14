#include <limits.h>
#include <string.h>

#include "geom_pass_tech.h"
#include "ogldev_util.h"

GeomPassTech::GeomPassTech()
{
}

bool GeomPassTech::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "lessons/shaders/geometry_pass.vs")) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "lessons/shaders/geometry_pass.fs")) {
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

void GeomPassTech::SetWVP(const Matrix4f& WVP)
{
	glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);
}