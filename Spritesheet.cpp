#include "Spritesheet.h"
#include "stb_image.h"
#include <iostream>

Spritesheet::Spritesheet() : Texture() {
	spritesheet_size = glm::ivec2(0.0f);
	tex_sprite_size = glm::ivec2(0.0f);
	get_size() = glm::ivec2(0.0f);
}

Spritesheet::Spritesheet(const char* filepath, glm::ivec2 sprite_dimensions) : Texture() {
	tex_sprite_size = sprite_dimensions;
	load_spritesheet(filepath);
	spritesheet_size = glm::vec2(get_size()) / glm::vec2(tex_sprite_size);
	sprite_size = glm::vec2(tex_sprite_size) / glm::vec2(get_size());
}

Spritesheet::~Spritesheet() {
	free();
}


void Spritesheet::load_spritesheet(std::string filepath) {
	load_texture(filepath, true);
}



int Spritesheet::get_sprite_index(int x_index, int y_index) {
	return y_index * (get_size().y / tex_sprite_size.y) + x_index * (get_size().x / tex_sprite_size.x);
}

glm::ivec2 Spritesheet::get_sprite_coords(int index) {
	glm::ivec2 r;

	int row_size = get_size().x / tex_sprite_size.x;

	r.x = index % row_size;
	r.y = index / row_size;

	return r;
}

//////////////////////////////////////////////////////////////////////////////

glm::vec2 Spritesheet::get_sprite_top_left(int index) {
	return get_sprite_pos(index);
}

glm::vec2 Spritesheet::get_sprite_top_right(int index) {
	return get_sprite_pos(index) + glm::vec2(get_sprite_size().x, 0.0f);
}

glm::vec2 Spritesheet::get_sprite_bottom_left(int index) {
	return get_sprite_pos(index) + glm::vec2(0.0f, -sprite_size.y);
}

glm::vec2 Spritesheet::get_sprite_bottom_right(int index) {
	return get_sprite_pos(index) + glm::vec2(sprite_size.x, -sprite_size.y);
}


//////////////////////////////////////////////////////////////////////////////

glm::vec2 Spritesheet::get_sprite_top_left(int x_index, int y_index) {
	return get_sprite_pos(x_index, y_index);
}

glm::vec2 Spritesheet::get_sprite_top_right(int x_index, int y_index) {
	return get_sprite_pos(x_index, y_index) + glm::vec2(get_sprite_size().x, 0.0f);
}

glm::vec2 Spritesheet::get_sprite_bottom_left(int x_index, int y_index) {
	return get_sprite_pos(x_index, y_index) + glm::vec2(0.0f, -sprite_size.y);
}

glm::vec2 Spritesheet::get_sprite_bottom_right(int x_index, int y_index) {
	return get_sprite_pos(x_index, y_index) + glm::vec2(sprite_size.x, -sprite_size.y);
}

//////////////////////////////////////////////////////////////////////////////

glm::vec2 Spritesheet::get_sprite_pos(int index) {
	glm::ivec2 coord = get_sprite_coords(index);

	return get_sprite_pos(coord.x, coord.y);
}

glm::vec2 Spritesheet::get_sprite_pos(int x_index, int y_index) {
	glm::vec2 r = sprite_size * glm::vec2(x_index, y_index);

	r.y = 1.0f - r.y;

	return r;
}

glm::vec2 Spritesheet::get_sprite_size() {
	return sprite_size;
}

// Operators
bool operator==(const Spritesheet& lhs, const Spritesheet& rhs) {
	return lhs.texture == rhs.texture &&
		lhs.spritesheet_size == rhs.spritesheet_size &&
		//lhs.tex_spritesheet_size == rhs.tex_spritesheet_size &&
		lhs.tex_sprite_size == rhs.tex_sprite_size;
}

bool operator!=(const Spritesheet& lhs, const Spritesheet& rhs) {
	return !(lhs == rhs);
}