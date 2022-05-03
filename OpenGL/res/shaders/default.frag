#version 330 core  

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

out vec4 color;

in vec2 v_TexCoord;
in float v_TexID;

uniform u_TexSamplers {
	uvec2 samplers[1]; 
};

void main() {
	
	int index = int(v_TexID);
//	color = texture(sampler2D(samplers[index]), v_TexCoord);
	
	sampler2D s = sampler2D(samplers[index]);
	
	color = texture(s, v_TexCoord);
};