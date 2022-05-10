#version 330 core  

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexID;

uniform u_TexSamplers {
	uvec2 samplers[1024];
};

void main() {  

	if (v_TexID < 0) { // if index < 0 do a color
		color = v_Color;
	}
	else { // else do a texture
		int index = int(v_TexID);
		sampler2D s = sampler2D(samplers[index]);
		color = texture(s, v_TexCoord);
	}
	

};