#pragma once

#ifndef GEOM_PASS_TECH_H
#define	GEOM_PASS_TECH_H

#include "technique.h"
#include "ogldev_math_3d.h"

class GeomPassTech : public Technique {
public:

	GeomPassTech();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
	void SetWVMatrix(const Matrix4f& WV);

private:

	GLuint m_WVPLocation;
	GLuint m_WVLocation;
};

#endif