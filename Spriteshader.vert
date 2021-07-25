#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex_Coord;

// Coordinate Matricies
uniform mat4 model;
//uniform mat4 model_inverse;
uniform mat4 view;
uniform mat4 projection;

out vec2 Tex_Coord;

void main() {
	//gl_Position = vec4(vPos, 1.0);
	Tex_Coord = vTex_Coord;

	vec3 FragPos = (model * vec4(vPos, 1.0)).xyz;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}