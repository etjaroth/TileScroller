#version 330 core
layout (location = 0) in vec2 vPos;
layout (location = 1) in vec2 vTex_Coord;

out vec2 Tex_Coord;

void main() {
	gl_Position = vec4(vPos, 0.0, 1.0);
	Tex_Coord = vTex_Coord;
}