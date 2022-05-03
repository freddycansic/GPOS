#version 330 core  
  
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require

layout(location = 0) in vec4 a_Position;  
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec2 a_Translation;
layout(location = 3) in float a_TexID;

out vec2 v_TexCoord;
out float v_TexID;

uniform mat4 u_ViewProj;

void main() {
	gl_Position = u_ViewProj * vec4(a_Position.x + a_Translation.x, a_Position.y + a_Translation.y, a_Position.z, a_Position.w);  

	v_TexID = a_TexID;
	v_TexCoord = a_TexCoord;
};