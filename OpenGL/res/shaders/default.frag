#version 430 core

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

out vec4 colour;

in vec4 v_Colour;
in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

struct Light 
{
	vec3 pos;
	vec3 colour;
};

layout (std140, binding = 0) uniform u_LightSources 
{
	Light u_Lights[16];
};

uniform uvec2 u_TexHandle;
uniform bool u_NoLighting; // passed in as int from CPU

const float ambientStrength = 0.4;

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
		Light light = u_Lights[0]; // TODO

		vec3 ambientColour = ambientStrength * light.colour;

		vec3 normal = normalize(v_Normal); // make 100% sure it's normalised
		vec3 lightDirection = normalize(light.pos - v_FragPos);

		float diffuseStrength = max(dot(normal, lightDirection), 0.0);
		vec3 diffuseColour = diffuseStrength * light.colour * 1.3;

		colour = vec4((ambientColour + diffuseColour) * colour.rgb, 1.0f);
	}
};