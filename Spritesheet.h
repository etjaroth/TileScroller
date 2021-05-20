#pragma once
#include "opengl_libs.h"
class Spritesheet
{
	GLuint texture = 0;
	glm::ivec2 size;
	glm::ivec2 sprite_size;
	glm::ivec2 size_in_texles;
	glm::ivec2 sprite_size_in_texles;
	
	bool flip = false;

public:
	Spritesheet();
	Spritesheet(const char* filepath, glm::ivec2 sprite_dimensions);

	~Spritesheet();

	void load_spritesheet(std::string filepath);
	void free_spritesheet();
	bool is_loaded();

	void bind_texture();

	glm::vec2 get_sprite_pos(int index);
	glm::vec2 get_sprite_size();
	glm::vec2 get_sprite_pos(int x_index, int y_index);

	// Operators
	friend bool operator==(const Spritesheet& lhs, const Spritesheet& rhs);
	friend bool operator!=(const Spritesheet& lhs, const Spritesheet& rhs);
};