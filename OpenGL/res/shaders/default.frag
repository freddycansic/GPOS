#version 330 core  
		   
out vec4 color;
in vec3 A_color;

void main() {  
	color = vec4(A_color, 1.0f);  
};