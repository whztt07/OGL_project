#include <limits.h>
#include <string.h>

#include "math_3d.h"
#include "lighting_technique.h"
#include "util.h"

static const char* pVS = "                                                                      \n\
#version 410 core                                                                               \n\
                                                                                                \n\
layout (location = 0) in vec3 Position_VS_in;                                                   \n\
layout (location = 1) in vec2 TexCoord_VS_in;                                                   \n\
layout (location = 2) in vec3 Normal_VS_in;                                                     \n\
                                                                                                \n\
uniform mat4 gWorld;                                                                            \n\
                                                                                                \n\
out vec3 WorldPos_CS_in;                                                                        \n\
out vec2 TexCoord_CS_in;                                                                        \n\
out vec3 Normal_CS_in;                                                                          \n\
                                                                                                \n\
void main()                                                                                     \n\
{                                                                                               \n\
    WorldPos_CS_in = (gWorld * vec4(Position_VS_in, 1.0)).xyz;                                  \n\
    TexCoord_CS_in = TexCoord_VS_in;                                                            \n\
    Normal_CS_in   = normalize((gWorld * vec4(Normal_VS_in, 0.0)).xyz);                         \n\
}";

static const char* pTessCS = "                                                                  \n\
#version 410 core                                                                               \n\
                                                                                                \n\
// define the number of CPs in the output patch                                                 \n\
layout (vertices = 1) out;                                                                      \n\
                                                                                                \n\
uniform float gTessellationLevel;                                                               \n\
                                                                                                \n\
// attributes of the input CPs                                                                  \n\
in vec3 WorldPos_CS_in[];                                                                       \n\
in vec2 TexCoord_CS_in[];                                                                       \n\
in vec3 Normal_CS_in[];                                                                         \n\
                                                                                                \n\
struct OutputPatch                                                                              \n\
{                                                                                               \n\
    vec3 WorldPos_B030;                                                                         \n\
    vec3 WorldPos_B021;                                                                         \n\
    vec3 WorldPos_B012;                                                                         \n\
    vec3 WorldPos_B003;                                                                         \n\
    vec3 WorldPos_B102;                                                                         \n\
    vec3 WorldPos_B201;                                                                         \n\
    vec3 WorldPos_B300;                                                                         \n\
    vec3 WorldPos_B210;                                                                         \n\
    vec3 WorldPos_B120;                                                                         \n\
    vec3 WorldPos_B111;                                                                         \n\
    vec3 Normal[3];                                                                             \n\
    vec2 TexCoord[3];                                                                           \n\
};                                                                                              \n\
                                                                                                \n\
// attributes of the output CPs                                                                 \n\
out patch OutputPatch oPatch;                                                                   \n\
                                                                                                \n\
vec3 ProjectToPlane(vec3 Point, vec3 PlanePoint, vec3 PlaneNormal)                              \n\
{                                                                                               \n\
    vec3 v = Point - PlanePoint;                                                                \n\
    float Len = dot(v, PlaneNormal);                                                            \n\
    vec3 d = Len * PlaneNormal;                                                                 \n\
    return (Point - d);                                                                         \n\
}                                                                                               \n\
                                                                                                \n\
                                                                                                \n\
void CalcPositions()                                                                            \n\
{                                                                                               \n\
    // The original vertices stay the same                                                      \n\
    oPatch.WorldPos_B030 = WorldPos_CS_in[0];                                                   \n\
    oPatch.WorldPos_B003 = WorldPos_CS_in[1];                                                   \n\
    oPatch.WorldPos_B300 = WorldPos_CS_in[2];                                                   \n\
                                                                                                \n\
    // Edges are names according to the opposing vertex                                         \n\
    vec3 EdgeB300 = oPatch.WorldPos_B003 - oPatch.WorldPos_B030;                                \n\
    vec3 EdgeB030 = oPatch.WorldPos_B300 - oPatch.WorldPos_B003;                                \n\
    vec3 EdgeB003 = oPatch.WorldPos_B030 - oPatch.WorldPos_B300;                                \n\
                                                                                                \n\
    // Generate two midpoints on each edge                                                      \n\
    oPatch.WorldPos_B021 = oPatch.WorldPos_B030 + EdgeB300 / 3.0;                               \n\
    oPatch.WorldPos_B012 = oPatch.WorldPos_B030 + EdgeB300 * 2.0 / 3.0;                         \n\
    oPatch.WorldPos_B102 = oPatch.WorldPos_B003 + EdgeB030 / 3.0;                               \n\
    oPatch.WorldPos_B201 = oPatch.WorldPos_B003 + EdgeB030 * 2.0 / 3.0;                         \n\
    oPatch.WorldPos_B210 = oPatch.WorldPos_B300 + EdgeB003 / 3.0;                               \n\
    oPatch.WorldPos_B120 = oPatch.WorldPos_B300 + EdgeB003 * 2.0 / 3.0;                         \n\
                                                                                                \n\
    // Project each midpoint on the plane defined by the nearest vertex and its normal          \n\
    oPatch.WorldPos_B021 = ProjectToPlane(oPatch.WorldPos_B021, oPatch.WorldPos_B030, oPatch.Normal[0]);     \n\
    oPatch.WorldPos_B012 = ProjectToPlane(oPatch.WorldPos_B012, oPatch.WorldPos_B003, oPatch.Normal[1]);     \n\
    oPatch.WorldPos_B102 = ProjectToPlane(oPatch.WorldPos_B102, oPatch.WorldPos_B003, oPatch.Normal[1]);     \n\
    oPatch.WorldPos_B201 = ProjectToPlane(oPatch.WorldPos_B201, oPatch.WorldPos_B300, oPatch.Normal[2]);     \n\
    oPatch.WorldPos_B210 = ProjectToPlane(oPatch.WorldPos_B210, oPatch.WorldPos_B300, oPatch.Normal[2]);     \n\
    oPatch.WorldPos_B120 = ProjectToPlane(oPatch.WorldPos_B120, oPatch.WorldPos_B030, oPatch.Normal[0]);     \n\
                                                                                                             \n\
    // Handle the center                                                                                     \n\
    vec3 Center = (oPatch.WorldPos_B003 + oPatch.WorldPos_B030 + oPatch.WorldPos_B300) / 3.0;                \n\
    oPatch.WorldPos_B111 = (oPatch.WorldPos_B021 + oPatch.WorldPos_B012 + oPatch.WorldPos_B102 +             \n\
                            oPatch.WorldPos_B201 + oPatch.WorldPos_B210 + oPatch.WorldPos_B120) / 6.0;       \n\
    oPatch.WorldPos_B111 += (oPatch.WorldPos_B111 - Center) / 2.0;                              \n\
}                                                                                               \n\
                                                                                                \n\
void main()                                                                                     \n\
{                                                                                               \n\
    // Set the control points of the output patch                                               \n\
    for (int i = 0 ; i < 3 ; i++) {                                                             \n\
        oPatch.Normal[i] = Normal_CS_in[i];                                                     \n\
        oPatch.TexCoord[i] = TexCoord_CS_in[i];                                                 \n\
    }                                                                                           \n\
                                                                                                \n\
    CalcPositions();                                                                            \n\
                                                                                                \n\
    // Calculate the tessellation levels                                                        \n\
    gl_TessLevelOuter[0] = gTessellationLevel;                                                  \n\
    gl_TessLevelOuter[1] = gTessellationLevel;                                                  \n\
    gl_TessLevelOuter[2] = gTessellationLevel;                                                  \n\
    gl_TessLevelInner[0] = gTessellationLevel;                                                  \n\
}                                                                                               \n\
";

static const char* pTessES = "                                                                  \n\
#version 410 core                                                                               \n\
                                                                                                \n\
layout(triangles, equal_spacing, ccw) in;                                                       \n\
                                                                                                \n\
uniform mat4 gVP;                                                                               \n\
                                                                                                \n\
struct OutputPatch                                                                              \n\
{                                                                                               \n\
    vec3 WorldPos_B030;                                                                         \n\
    vec3 WorldPos_B021;                                                                         \n\
    vec3 WorldPos_B012;                                                                         \n\
    vec3 WorldPos_B003;                                                                         \n\
    vec3 WorldPos_B102;                                                                         \n\
    vec3 WorldPos_B201;                                                                         \n\
    vec3 WorldPos_B300;                                                                         \n\
    vec3 WorldPos_B210;                                                                         \n\
    vec3 WorldPos_B120;                                                                         \n\
    vec3 WorldPos_B111;                                                                         \n\
    vec3 Normal[3];                                                                             \n\
    vec2 TexCoord[3];                                                                           \n\
};                                                                                              \n\
                                                                                                \n\
in patch OutputPatch oPatch;                                                                    \n\
                                                                                                \n\
out vec3 WorldPos_FS_in;                                                                        \n\
out vec2 TexCoord_FS_in;                                                                        \n\
out vec3 Normal_FS_in;                                                                          \n\
                                                                                                \n\
vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)                                                   \n\
{                                                                                               \n\
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;   \n\
}                                                                                               \n\
                                                                                                \n\
vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)                                                   \n\
{                                                                                               \n\
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;   \n\
}                                                                                               \n\
                                                                                                \n\
void main()                                                                                     \n\
{                                                                                               \n\
    // Interpolate the attributes of the output vertex using the barycentric coordinates        \n\
    TexCoord_FS_in = interpolate2D(oPatch.TexCoord[0], oPatch.TexCoord[1], oPatch.TexCoord[2]); \n\
    Normal_FS_in = interpolate3D(oPatch.Normal[0], oPatch.Normal[1], oPatch.Normal[2]);         \n\
                                                                                                \n\
    float u = gl_TessCoord.x;                                                                   \n\
    float v = gl_TessCoord.y;                                                                   \n\
    float w = gl_TessCoord.z;                                                                   \n\
    float uPow3 = pow(u, 3);                                                                    \n\
    float vPow3 = pow(v, 3);                                                                    \n\
    float wPow3 = pow(w, 3);                                                                    \n\
    float uPow2 = pow(u, 2);                                                                    \n\
    float vPow2 = pow(v, 2);                                                                    \n\
    float wPow2 = pow(w, 2);                                                                    \n\
    WorldPos_FS_in = oPatch.WorldPos_B300 * wPow3 + oPatch.WorldPos_B030 * uPow3 + oPatch.WorldPos_B003 * vPow3 +                               \n\
                     oPatch.WorldPos_B210 * 3.0 * wPow2 * u + oPatch.WorldPos_B120 * 3.0 * w * uPow2 + oPatch.WorldPos_B201 * 3.0 * wPow2 * v + \n\
                     oPatch.WorldPos_B021 * 3.0 * uPow2 * v + oPatch.WorldPos_B102 * 3.0 * w * vPow2 + oPatch.WorldPos_B012 * 3.0 * u * vPow2 + \n\
                     oPatch.WorldPos_B111 * 6.0 * w * u * v;                                    \n\
    gl_Position = gVP * vec4(WorldPos_FS_in, 1.0);                                              \n\
}                                                                                               \n\
";

static const char* pFS = "                                                                  \n\
#version 410 core                                                                           \n\
                                                                                            \n\
const int MAX_POINT_LIGHTS = 2;                                                             \n\
const int MAX_SPOT_LIGHTS = 2;                                                              \n\
                                                                                            \n\
in vec2 TexCoord_FS_in;                                                                     \n\
in vec3 Normal_FS_in;                                                                       \n\
in vec3 WorldPos_FS_in;                                                                     \n\
                                                                                            \n\
out vec4 FragColor;                                                                         \n\
                                                                                            \n\
struct BaseLight                                                                            \n\
{                                                                                           \n\
    vec3 Color;                                                                             \n\
    float AmbientIntensity;                                                                 \n\
    float DiffuseIntensity;                                                                 \n\
};                                                                                          \n\
                                                                                            \n\
struct DirectionalLight                                                                     \n\
{                                                                                           \n\
    struct BaseLight Base;                                                                  \n\
    vec3 Direction;                                                                         \n\
};                                                                                          \n\
                                                                                            \n\
struct Attenuation                                                                          \n\
{                                                                                           \n\
    float Constant;                                                                         \n\
    float Linear;                                                                           \n\
    float Exp;                                                                              \n\
};                                                                                          \n\
                                                                                            \n\
struct PointLight                                                                           \n\
{                                                                                           \n\
    struct BaseLight Base;                                                                  \n\
    vec3 Position;                                                                          \n\
    Attenuation Atten;                                                                      \n\
};                                                                                          \n\
                                                                                            \n\
struct SpotLight                                                                            \n\
{                                                                                           \n\
    struct PointLight Base;                                                                 \n\
    vec3 Direction;                                                                         \n\
    float Cutoff;                                                                           \n\
};                                                                                          \n\
                                                                                            \n\
uniform int gNumPointLights;                                                                \n\
uniform int gNumSpotLights;                                                                 \n\
uniform DirectionalLight gDirectionalLight;                                                 \n\
uniform PointLight gPointLights[MAX_POINT_LIGHTS];                                          \n\
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS];                                             \n\
uniform sampler2D gColorMap;                                                                \n\
uniform vec3 gEyeWorldPos;                                                                  \n\
uniform float gMatSpecularIntensity;                                                        \n\
uniform float gSpecularPower;                                                               \n\
                                                                                            \n\
vec4 CalcLightInternal(struct BaseLight Light, vec3 LightDirection, vec3 Normal)            \n\
{                                                                                           \n\
    vec4 AmbientColor = vec4(Light.Color, 1.0f) * Light.AmbientIntensity;                   \n\
    float DiffuseFactor = dot(Normal, -LightDirection);                                     \n\
                                                                                            \n\
    vec4 DiffuseColor  = vec4(0, 0, 0, 0);                                                  \n\
    vec4 SpecularColor = vec4(0, 0, 0, 0);                                                  \n\
                                                                                            \n\
    if (DiffuseFactor > 0) {                                                                \n\
        DiffuseColor = vec4(Light.Color, 1.0f) * Light.DiffuseIntensity * DiffuseFactor;    \n\
                                                                                            \n\
        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos_FS_in);                        \n\
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));                     \n\
        float SpecularFactor = dot(VertexToEye, LightReflect);                              \n\
        SpecularFactor = pow(SpecularFactor, gSpecularPower);                               \n\
        if (SpecularFactor > 0) {                                                           \n\
            SpecularColor = vec4(Light.Color, 1.0f) *                                       \n\
                            gMatSpecularIntensity * SpecularFactor;                         \n\
        }                                                                                   \n\
    }                                                                                       \n\
                                                                                            \n\
    return (AmbientColor + DiffuseColor + SpecularColor);                                   \n\
}                                                                                           \n\
                                                                                            \n\
vec4 CalcDirectionalLight(vec3 Normal)                                                      \n\
{                                                                                           \n\
    return CalcLightInternal(gDirectionalLight.Base, gDirectionalLight.Direction, Normal);  \n\
}                                                                                           \n\
                                                                                            \n\
vec4 CalcPointLight(PointLight l, vec3 Normal)                                       \n\
{                                                                                           \n\
    vec3 LightDirection = WorldPos_FS_in - l.Position;                                      \n\
    float Distance = length(LightDirection);                                                \n\
    LightDirection = normalize(LightDirection);                                             \n\
                                                                                            \n\
    vec4 Color = CalcLightInternal(l.Base, LightDirection, Normal);                         \n\
    float Attenuation =  l.Atten.Constant +                                                 \n\
                         l.Atten.Linear * Distance +                                        \n\
                         l.Atten.Exp * Distance * Distance;                                 \n\
                                                                                            \n\
    return Color / Attenuation;                                                             \n\
}                                                                                           \n\
                                                                                            \n\
vec4 CalcSpotLight(SpotLight l, vec3 Normal)                                         \n\
{                                                                                           \n\
    vec3 LightToPixel = normalize(WorldPos_FS_in - l.Base.Position);                        \n\
    float SpotFactor = dot(LightToPixel, l.Direction);                                      \n\
                                                                                            \n\
    if (SpotFactor > l.Cutoff) {                                                            \n\
        vec4 Color = CalcPointLight(l.Base, Normal);                                        \n\
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - l.Cutoff));                   \n\
    }                                                                                       \n\
    else {                                                                                  \n\
        return vec4(0,0,0,0);                                                               \n\
    }                                                                                       \n\
}                                                                                           \n\
                                                                                            \n\
void main()                                                                                 \n\
{                                                                                           \n\
    vec3 Normal = normalize(Normal_FS_in);                                                  \n\
    vec4 TotalLight = CalcDirectionalLight(Normal);                                         \n\
                                                                                            \n\
    for (int i = 0 ; i < gNumPointLights ; i++) {                                           \n\
        TotalLight += CalcPointLight(gPointLights[i], Normal);                              \n\
    }                                                                                       \n\
                                                                                            \n\
    for (int i = 0 ; i < gNumSpotLights ; i++) {                                            \n\
        TotalLight += CalcSpotLight(gSpotLights[i], Normal);                                \n\
    }                                                                                       \n\
                                                                                            \n\
    FragColor = texture(gColorMap, TexCoord_FS_in.xy) * TotalLight;                         \n\
}";

LightingTechnique::LightingTechnique()
{
}

bool LightingTechnique::Init()
{
	if (!Technique::Init()) {
		return false;
	}

	if (!AddShader(GL_VERTEX_SHADER, pVS)) {
		return false;
	}

	if (!AddShader(GL_TESS_CONTROL_SHADER, pTessCS)) {
		return false;
	}

	if (!AddShader(GL_TESS_EVALUATION_SHADER, pTessES)) {
		return false;
	}

	if (!AddShader(GL_FRAGMENT_SHADER, pFS)) {
		return false;
	}

	if (!Finalize()) {
		return false;
	}

	m_VPLocation = GetUniformLocation("gVP");
	m_WorldMatrixLocation = GetUniformLocation("gWorld");
	m_colorTextureLocation = GetUniformLocation("gColorMap");
	m_eyeWorldPosLocation = GetUniformLocation("gEyeWorldPos");
	m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Base.Color");
	m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.Base.AmbientIntensity");
	m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
	m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
	m_matSpecularIntensityLocation = GetUniformLocation("gMatSpecularIntensity");
	m_matSpecularPowerLocation = GetUniformLocation("gSpecularPower");
	m_numPointLightsLocation = GetUniformLocation("gNumPointLights");
	m_numSpotLightsLocation = GetUniformLocation("gNumSpotLights");
	m_TLLocation = GetUniformLocation("gTessellationLevel");

	if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
		m_VPLocation == INVALID_UNIFORM_LOCATION ||
		m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
		m_colorTextureLocation == INVALID_UNIFORM_LOCATION ||
		m_eyeWorldPosLocation == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
		m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION ||
		m_matSpecularIntensityLocation == INVALID_UNIFORM_LOCATION ||
		m_matSpecularPowerLocation == INVALID_UNIFORM_LOCATION ||
		m_numPointLightsLocation == INVALID_UNIFORM_LOCATION ||
		m_numSpotLightsLocation == INVALID_UNIFORM_LOCATION ||
		m_TLLocation == INVALID_UNIFORM_LOCATION) {
		return false;
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pointLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		snprintf(Name, sizeof(Name), "gPointLights[%d].Base.Color", i);
		m_pointLightsLocation[i].Color = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Base.AmbientIntensity", i);
		m_pointLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Position", i);
		m_pointLightsLocation[i].Position = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Base.DiffuseIntensity", i);
		m_pointLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Constant", i);
		m_pointLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Linear", i);
		m_pointLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gPointLights[%d].Atten.Exp", i);
		m_pointLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		if (m_pointLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			m_pointLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
			return false;
		}
	}

	for (unsigned int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_spotLightsLocation); i++) {
		char Name[128];
		memset(Name, 0, sizeof(Name));
		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
		m_spotLightsLocation[i].Color = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
		m_spotLightsLocation[i].AmbientIntensity = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
		m_spotLightsLocation[i].Position = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
		m_spotLightsLocation[i].Direction = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
		m_spotLightsLocation[i].Cutoff = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
		m_spotLightsLocation[i].DiffuseIntensity = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
		m_spotLightsLocation[i].Atten.Constant = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
		m_spotLightsLocation[i].Atten.Linear = GetUniformLocation(Name);

		snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
		m_spotLightsLocation[i].Atten.Exp = GetUniformLocation(Name);

		if (m_spotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
			m_spotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
			return false;
		}
	}

	return true;
}

void LightingTechnique::SetVP(const Matrix4f& VP)
{
	glUniformMatrix4fv(m_VPLocation, 1, GL_TRUE, (const GLfloat*)VP.m);
}

void LightingTechnique::SetWorldMatrix(const Matrix4f& WorldInverse)
{
	glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
}

void LightingTechnique::SetColorTextureUnit(unsigned int TextureUnit)
{
	glUniform1i(m_colorTextureLocation, TextureUnit);
}

void LightingTechnique::SetDirectionalLight(const DirectionalLight& Light)
{
	glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
	glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
	Vector3f Direction = Light.Direction;
	Direction.Normalize();
	glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
	glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
}

void LightingTechnique::SetEyeWorldPos(const Vector3f& EyeWorldPos)
{
	glUniform3f(m_eyeWorldPosLocation, EyeWorldPos.x, EyeWorldPos.y, EyeWorldPos.z);
}

void LightingTechnique::SetMatSpecularIntensity(float Intensity)
{
	glUniform1f(m_matSpecularIntensityLocation, Intensity);
}

void LightingTechnique::SetMatSpecularPower(float Power)
{
	glUniform1f(m_matSpecularPowerLocation, Power);
}

void LightingTechnique::SetPointLights(unsigned int NumLights, const PointLight* pLights)
{
	glUniform1i(m_numPointLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(m_pointLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_pointLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		glUniform1f(m_pointLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}

void LightingTechnique::SetSpotLights(unsigned int NumLights, const SpotLight* pLights)
{
	glUniform1i(m_numSpotLightsLocation, NumLights);

	for (unsigned int i = 0; i < NumLights; i++) {
		glUniform3f(m_spotLightsLocation[i].Color, pLights[i].Color.x, pLights[i].Color.y, pLights[i].Color.z);
		glUniform1f(m_spotLightsLocation[i].AmbientIntensity, pLights[i].AmbientIntensity);
		glUniform1f(m_spotLightsLocation[i].DiffuseIntensity, pLights[i].DiffuseIntensity);
		glUniform3f(m_spotLightsLocation[i].Position, pLights[i].Position.x, pLights[i].Position.y, pLights[i].Position.z);
		Vector3f Direction = pLights[i].Direction;
		Direction.Normalize();
		glUniform3f(m_spotLightsLocation[i].Direction, Direction.x, Direction.y, Direction.z);
		glUniform1f(m_spotLightsLocation[i].Cutoff, cosf(ToRadian(pLights[i].Cutoff)));
		glUniform1f(m_spotLightsLocation[i].Atten.Constant, pLights[i].Attenuation.Constant);
		glUniform1f(m_spotLightsLocation[i].Atten.Linear, pLights[i].Attenuation.Linear);
		glUniform1f(m_spotLightsLocation[i].Atten.Exp, pLights[i].Attenuation.Exp);
	}
}

void LightingTechnique::SetTessellationLevel(float TL)
{
	glUniform1f(m_TLLocation, TL);
}