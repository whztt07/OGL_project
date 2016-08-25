#pragma once

#ifndef BILLBOARD_LIST_H
#define	BILLBOARD_LIST_H

#include <string>

#include "ogldev_texture.h"
#include "billboard_technique.h"

class BillboardList
{
public:
	BillboardList();
	~BillboardList();

	bool Init(const std::string& TexFilename);

	void Render(const Matrix4f& VP, const Vector3f& CameraPos);

private:
	void CreatePositionBuffer();

	GLuint m_VB;
	Texture* m_pTexture;
	BillboardTechnique m_technique;
};

#endif	/* BILLBOARD_LIST_H */