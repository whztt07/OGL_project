#pragma once

#ifndef SSAO_TECHNIQUE_H
#define	SSAO_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"
#include "ogldev_io_buffer.h"

class SSAOTechnique : public Technique {
public:

	SSAOTechnique();

	virtual bool Init();

	void BindPositionBuffer(IOBuffer& posBuf);
	void SetSampleRadius(float sr);
	void SetProjMatrix(const Matrix4f& m);

private:

	void GenKernel();

	const static uint KERNEL_SIZE = 64;

	GLuint m_posTextureUnitLocation;
	GLuint m_sampleRadLocation;
	GLuint m_kernelLocation;
	GLuint m_projMatrixLocation;
};

#endif	/* SSAO_TECHNIQUE_H */