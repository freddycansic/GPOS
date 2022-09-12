#version 430 core

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : enable

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

//#ifdef GL_ARB_gpu_shader_int64
	layout(bindless_sampler) uniform sampler2D u_TexHandle;
//#else
//	uniform uvec2 u_TexHandle;
//#endif

void main() {
	
	if (uvec2(u_TexHandle) == uvec2(0)) { // if index < 0 do a color
		color = v_Color;
	}
	else { // else do a texture
		color = texture(sampler2D(u_TexHandle), v_TexCoord);
	}

};