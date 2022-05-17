#version 430 core  

#extension GL_ARB_bindless_texture : require

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

layout(bindless_sampler) uniform sampler2D u_TexHandle;

void main() {  
	
	if (uvec2(u_TexHandle) == uvec2(0)) { // if index < 0 do a color
		color = v_Color;
	}
	else { // else do a texture
		color = texture(u_TexHandle, v_TexCoord);
	}

};