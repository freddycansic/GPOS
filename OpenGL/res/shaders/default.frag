#version 330 core  

#extension GL_ARB_bindless_texture : require
#extension GL_ARB_gpu_shader_int64 : require
#extension GL_EXT_vertex_attrib_64bit : require

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoord;
in flat uint64_t v_TexHandle;

void main() {  
	
	if (v_TexHandle < 0) { // if index < 0 do a color
		color = v_Color;
	}
	else { // else do a texture
		sampler2D s = sampler2D(uvec2(v_TexHandle));
		color = texture(s, v_TexCoord);
	}

};