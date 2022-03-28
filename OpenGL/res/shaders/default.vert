#version 330 core  
  
layout(location = 0) in vec4 position;  
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord; // v = varying

uniform mat4 u_ModelViewProj;

void main() {  
	gl_Position = u_ModelViewProj * position;  
	v_TexCoord = texCoord;

};