#pragma once
#include "opengl_libs.h"

class Texture
{
	GLuint texture = 0;
	glm::ivec2 texture_dimensions;

public:
	Texture();

	void generate_texture(glm::ivec2 size);
	void load_texture(std::string filepath, bool flip);
	bool is_loaded();
	void free();

	void bind();
	void unbind();

	glm::ivec2 get_size();
};