#pragma once

#ifndef LIGHTING_TECHNIQUE_H
#define	LIGHTING_TECHNIQUE_H

#include "technique.h"
#include "ogldev_math_3d.h"
#include "ogldev_lights_common.h"
#include "csm_technique.h"

#define NUM_CASCADES 3

class LightingTechnique : public Technique
{
public:

	static const uint MAX_POINT_LIGHTS = 2;
	static const uint MAX_SPOT_LIGHTS = 2;

	LightingTechnique();

	virtual bool Init();

	void SetWVP(const Matrix4f& WVP);
	void SetLightWVP(uint CascadeIndex, const Matrix4f& LightWVP);
	void SetCascadeEndClipSpace(uint CascadeIndex, float End);
	void SetWorldMatrix(const Matrix4f& WVP);
	void SetColorTextureUnit(uint TextureUnit);
	void SetShadowMapTextureUnit();
	void SetDirectionalLight(const DirectionalLight& Light);
	void SetPointLights(uint NumLights, const PointLight* pLights);
	void SetSpotLights(uint NumLights, const SpotLight* pLights);
	void SetEyeWorldPos(const Vector3f& EyeWorldPos);
	void SetMatSpecularIntensity(float Intensity);
	void SetMatSpecularPower(float Power);
	void SetShadowMapSize(float Width, float Height);

private:

	GLuint m_WVPLocation;
	GLuint m_lightWVPLocation[NUM_CASCADES];
	GLuint m_cascadeEndClipSpace[NUM_CASCADES];
	GLuint m_worldMatrixLocation;
	GLuint m_samplerLocation;
	GLuint m_shadowMapLocation[NUM_CASCADES];
	GLuint m_eyeWorldPosLocation;
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	GLuint m_numPointLightsLocation;
	GLuint m_numSpotLightsLocation;
	GLuint m_shadowMapSizeLocation;

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
};

#endif	/* LIGHTING_TECHNIQUE_H */