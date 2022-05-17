#version 430 core  

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

uniform uint64_t u_TexHandle;

void main() {  
	
	if (u_TexHandle == 0) { // if index < 0 do a color
		color = v_Color;
	}
	else { // else do a texture
		sampler2D s = sampler2D(uvec2(u_TexHandle));
		color = texture(s, v_TexCoord);
	}

};