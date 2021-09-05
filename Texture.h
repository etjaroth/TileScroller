#pragma once
#include "opengl_libs.h"

class Texture
{
	GLuint texture = 0;
	glm::ivec2 texture_dimensions;

	static GLuint currently_bound_texture;
	void auto_bind_texture();

public:
	Texture();

	void generate_texture(glm::ivec2 size, void* data=NULL, unsigned int data_size=0);
	void load_texture(std::string filepath, bool flip);
	bool is_loaded();
	void free();

	void bind();
	void unbind();

	glm::ivec2 get_size();

	// opengl settings
		// border_wrap
	void set_texture_border_wrap(GLenum style);
	void set_texture_border_wrap_x(GLenum style);
	void set_texture_border_wrap_y(GLenum style);
	GLenum get_texture_border_wrap_x();
	GLenum get_texture_border_wrap_y();
};