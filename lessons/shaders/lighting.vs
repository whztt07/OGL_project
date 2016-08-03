#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

out vec2 TexCoord0;
out vec3 Normal0;
out vec3 WorldPos0;
out vec4 LightSpacePos0;

uniform mat4 gWVP;                                                  
uniform mat4 gLightWVP;
uniform mat4 gWorld;                                                
                                                                                    
void main()
{                                                                                   
    gl_Position     = gWVP * vec4(Position, 1.0);                                        
    TexCoord0      = TexCoord;                                                         
    Normal0        = (gWorld * vec4(Normal, 0.0)).xyz;                                  
    WorldPos0      = (gWorld * vec4(Position, 1.0)).xyz;                                
    LightSpacePos0 = gLightWVP * vec4(Position, 1.0);
}