#version 330 core  

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexID;

uniform sampler2D u_Textures[32];

void main() {  

	int index = int(v_TexID);

	if (index >= 0) { // if index != -1 do a texture
		color = texture(u_Textures[index], v_TexCoord);
	}
	else { // else do a color
		color = v_Color;
	}


};