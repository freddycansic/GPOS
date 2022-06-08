#version 430 core  

#extension GL_ARB_bindless_texture : enable

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;

layout(bindless_sampler) uniform uvec2 u_TexHandle;

void main() {  
	
	if (u_TexHandle == uvec2(0)) { // if index < 0 do a color
		color = v_Color;
	}
	else { // else do a texture
		color = texture(sampler2D(u_TexHandle), v_TexCoord);
	}

};