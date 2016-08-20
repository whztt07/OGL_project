#include "billboard_technique.h"
#include "ogldev_util.h"

BillboardTechnique::BillboardTechnique()
{
}

bool BillboardTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "engine/shaders/billboard.vs")) {
		return false;
	}

	if (!AddShader(GL_GEOMETRY_SHADER, "engine/shaders/billboard.gs")) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "engine/shaders/billboard.fs")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_VPLocation = GetUniformLocation("gVP");
	m_cameraPosLocation = GetUniformLocation("gCameraPos");
	m_colorMapLocation = GetUniformLocation("gColorMap");

	if (m_VPLocation == INVALID_UNIFORM_LOCATION ||
		m_cameraPosLocation == INVALID_UNIFORM_LOCATION ||
		m_colorMapLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void BillboardTechnique::SetVP(const Matrix4f& VP)
{
	glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, (const GLfloat*)VP.m);
}

void BillboardTechnique::SetCameraPosition(const Vector3f& Pos)
{
	glUniform3f(m_cameraPosLocation, Pos.x, Pos.y, Pos.z);
}

void BillboardTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorMapLocation, TextureUnit);
}