#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTex_Coord;

out vec2 Tex_Coord;

void main() {
	gl_Position = vec4(vPos, 1.0);
	Tex_Coord = vTex_Coord;
}