#version 330 core  
  
layout(location = 0) in vec4 position;  
layout(location = 1) in vec3 color;  

out vec3 A_color;

void main() {  
	A_color = color;
	
	gl_Position = position;  
} ;