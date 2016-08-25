#pragma once

#ifndef OGLDEV_CUBEMAP_TEXTURE_H
#define	OGLDEV_CUBEMAP_TEXTURE_H

#include <string>
#include <GL/glew.h>
#include <ImageMagick/Magick++.h>

using namespace std;

class CubemapTexture
{
public:

	CubemapTexture(const string& Directory,
		const string& PosXFilename,
		const string& NegXFilename,
		const string& PosYFilename,
		const string& NegYFilename,
		const string& PosZFilename,
		const string& NegZFilename);

	~CubemapTexture();

	bool Load();

	void Bind(GLenum TextureUnit);

private:

	string m_fileNames[6];
	GLuint m_textureObj;
};

#endif	/* OGLDEV_CUBEMAP_TEXTURE_H */