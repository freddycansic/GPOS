#version 330 core  
  
layout(location = 0) in vec4 a_Position;  
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Translation;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexID;

uniform mat4 u_ViewProj;

void main() {
	gl_Position = u_ViewProj * vec4(a_Position.x + a_Translation.x, a_Position.y + a_Translation.y, a_Position.z, a_Position.w);  

	v_Color = a_Color;
};