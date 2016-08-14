#pragma once

#ifndef SHADOWMAPFBO_H
#define	SHADOWMAPFBO_H

#include <GL/glew.h>

class PickingTexture
{
public:
	PickingTexture();

	~PickingTexture();

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

	void EnableWriting();

	void DisableWriting();

	struct PixelInfo {
		unsigned int ObjectID;
		unsigned int DrawID;
		unsigned int PrimID;

		PixelInfo()
		{
			ObjectID = 0;
			DrawID = 0;
			PrimID = 0;
		}
	};

	PixelInfo ReadPixel(unsigned int x, unsigned int y);

private:
	GLuint m_fbo;
	GLuint m_pickingTexture;
	GLuint m_depthTexture;
};

#endif	/* SHADOWMAPFBO_H */