#version 330 core  

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

out vec4 color;

in vec2 v_TexCoord;
in float v_TexID;

uniform u_TexSamplers {
	uint64_t samplers[1024]; 
};

void main() {  

	int index = int(v_TexID);
	color = texture(sampler2D(samplers[index]), v_TexCoord);

};