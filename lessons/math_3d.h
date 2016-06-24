#pragma once

#ifndef MATH_3D_H
#define	MATH_3D_H

#define _USE_MATH_DEFINES
#include <cstdio>
#include <cmath>

#define ToRadian(x) ((x) * M_PI / 180.0)
#define ToDegree(x) ((x) * 180.0 / M_PI)

struct Vector3f
{
	float x;
	float y;
	float z;

	Vector3f()
	{
	}

	Vector3f(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f Cross(const Vector3f& v) const;

	Vector3f& Normalize();

	void Print() const
	{
		printf("(%.02f, %.02f, %.02f", x, y, z);
	}
};

class Matrix4f
{
public:
	float m[4][4];

	Matrix4f()
	{
	}


	inline void InitIdentity()
	{
		m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
		m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
		m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
		m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
	}

	inline Matrix4f operator*(const Matrix4f& Right) const
	{
		Matrix4f Ret;

		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				Ret.m[i][j] =	m[i][0] * Right.m[0][j] +
								m[i][1] * Right.m[1][j] +
								m[i][2] * Right.m[2][j] +
								m[i][3] * Right.m[3][j];
			}
		}

		return Ret;
	}

	void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
	void InitRotateTransform(float RotateX, float RotateY, float RotateZ);
	void InitTranslationTransform(float x, float y, float z);
	void InitCameraTransform(const Vector3f& Target, const Vector3f& Up);
	void InitPersProjTransform(float FOV, float Width, float Height, float zNear, float zFar);
};


#endif	/* MATH_3D_H */
