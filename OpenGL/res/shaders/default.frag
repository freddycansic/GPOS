#version 330 core  

out vec4 color;

in vec2 v_TexCoord;
in float v_TexID;

uniform sampler2D u_Textures[2];

void main() {  

	int index = int(v_TexID);
	color = texture(u_Textures[index], v_TexCoord);

};