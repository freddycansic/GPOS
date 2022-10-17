#version 430 core

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

out vec4 colour;

in vec4 v_Colour;
in vec2 v_TexCoord;

layout (std140, binding = 0) uniform u_LightSources 
{
	vec3 u_Lights[16];
};

uniform uvec2 u_TexHandle;
uniform bool u_NoLighting; // passed in as int from CPU

const float ambientStrength = 0.1;

void main() 
{
	// if index = 0 do a color aka invalid texture
	if (uvec2(u_TexHandle) == uvec2(0)) 
	{
		colour = v_Colour;
	}
	else 
	{ // else do a texture
		colour = mix(v_Colour, texture(sampler2D(u_TexHandle), v_TexCoord), 0.5);
	}

	if (!u_NoLighting) 
	{
		vec3 ambient = ambientStrength * u_Lights[0];
		colour = colour * vec4(ambient, 1.0f);
	}
};