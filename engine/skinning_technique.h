#pragma once

#ifndef SKINNING_TECHNIQUE_H
#define	SKINNING_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"
#include "ogldev_lights_common.h"

class SkinningTechnique : public Technique {
public:

	static const uint MAX_POINT_LIGHTS = 2;
	static const uint MAX_SPOT_LIGHTS = 2;
	static const uint MAX_BONES = 100;

	SkinningTechnique();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
	void SetPrevWVP(const Matrix4f& PrevWVP);
	void SetWorldMatrix(const Matrix4f& WVP);
	void SetColorTextureUnit(uint TextureUnit);
	void SetDirectionalLight(const DirectionalLight& Light);
	void SetPointLights(uint NumLights, const PointLight* pLights);
	void SetSpotLights(uint NumLights, const SpotLight* pLights);
	void SetEyeWorldPos(const Vector3f& EyeWorldPos);
	void SetMatSpecularIntensity(float Intensity);
	void SetMatSpecularPower(float Power);
	void SetBoneTransform(uint Index, const Matrix4f& Transform);
	void SetPrevBoneTransform(uint Index, const Matrix4f& Transform);

private:

	GLuint m_WVPLocation;
	GLuint m_WorldMatrixLocation;
	GLuint m_colorTextureLocation;
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_numPointLightsLocation;
	GLuint m_numSpotLightsLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Direction;
	} m_dirLightLocation;

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_pointLightsLocation[MAX_POINT_LIGHTS];

	struct {
		GLuint Color;
		GLuint AmbientIntensity;
		GLuint DiffuseIntensity;
		GLuint Position;
		GLuint Direction;
		GLuint Cutoff;
		struct {
			GLuint Constant;
			GLuint Linear;
			GLuint Exp;
		} Atten;
	} m_spotLightsLocation[MAX_SPOT_LIGHTS];

	GLuint m_boneLocation[MAX_BONES];
	GLuint m_prevBoneLocation[MAX_BONES];
};

#endif	/* SKINNING_TECHNIQUE_H */