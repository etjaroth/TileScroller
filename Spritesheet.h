#pragma once
#include "opengl_libs.h"
#include "Texture.h"

class Spritesheet : public Texture
{
	//GLuint texture = 0;
	glm::ivec2 spritesheet_size;
	glm::ivec2 tex_sprite_size;

	glm::vec2 sprite_size;
	
	bool flip = true;

public:
	Spritesheet();
	Spritesheet(const char* filepath, glm::ivec2 sprite_dimensions);

	~Spritesheet();

	void load_spritesheet(std::string filepath);

	int get_sprite_index(int x_index, int y_index);
	glm::ivec2 get_sprite_coords(int index);

	glm::vec2 get_sprite_top_left(int index);
	glm::vec2 get_sprite_top_right(int index);
	glm::vec2 get_sprite_bottom_left(int index);
	glm::vec2 get_sprite_bottom_right(int index);

	glm::vec2 get_sprite_top_left(int x_index, int y_index);
	glm::vec2 get_sprite_top_right(int x_index, int y_index);
	glm::vec2 get_sprite_bottom_left(int x_index, int y_index);
	glm::vec2 get_sprite_bottom_right(int x_index, int y_index);

	glm::vec2 get_sprite_size();
	glm::vec2 get_sprite_pos(int index);
	glm::vec2 get_sprite_pos(int x_index, int y_index);

	// Operators
	friend bool operator==(const Spritesheet& lhs, const Spritesheet& rhs);
	friend bool operator!=(const Spritesheet& lhs, const Spritesheet& rhs);
};