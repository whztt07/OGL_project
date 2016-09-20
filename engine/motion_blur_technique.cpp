#include "motion_blur_technique.h"
#include "ogldev_util.h"

MotionBlurTechnique::MotionBlurTechnique()
{
}

bool MotionBlurTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, "engine/shaders/motion_blur.vs")) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, "engine/shaders/motion_blur.fs")) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_colorTextureLocation = GetUniformLocation("gColorTexture");
	m_motionTextureLocation = GetUniformLocation("gMotionTexture");

	if (m_motionTextureLocation == INVALID_UNIFORM_LOCATION ||
		m_colorTextureLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	return true;
}

void MotionBlurTechnique::SetColorTextureUnit(uint TextureUnit)
{
	glUniform1i(m_colorTextureLocation, TextureUnit);
}

void MotionBlurTechnique::SetMotionTextureUnit(uint TextureUnit)
{
	glUniform1i(m_motionTextureLocation, TextureUnit);
}