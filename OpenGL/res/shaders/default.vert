#version 430 core  
  
#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require
#extension GL_ARB_gpu_shader_fp64 : require
#extension GL_ARB_gpu_shader5 : require
#extension GL_ARB_vertex_attrib_64bit : require

layout(location = 0) in vec4 a_Position;  
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

out vec4 v_Color;
out vec2 v_TexCoord; // v = varying

uniform mat4 u_ViewProj;

void main() {
	gl_Position = u_ViewProj * a_Position;  

	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
};