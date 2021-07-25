#include "Spritesheet.h"
#include "stb_image.h"
#include <iostream>

Spritesheet::Spritesheet() {
	spritesheet_size = glm::ivec2(0.0f);
	tex_sprite_size = glm::ivec2(0.0f);
	tex_spritesheet_size = glm::ivec2(0.0f);
}

Spritesheet::Spritesheet(const char* filepath, glm::ivec2 sprite_dimensions) {
	tex_sprite_size = sprite_dimensions;
	load_spritesheet(filepath);
	spritesheet_size = glm::vec2(tex_spritesheet_size) / glm::vec2(tex_sprite_size);
	sprite_size = glm::vec2(tex_sprite_size) / glm::vec2(tex_spritesheet_size);
}

Spritesheet::~Spritesheet() {
	free_spritesheet();
}


void Spritesheet::load_spritesheet(std::string filepath) {
	if (is_loaded()) {
		free_spritesheet();
	}

	stbi_set_flip_vertically_on_load(flip);
	
	int nrChannels;
	const GLenum colortype = GL_RGBA;
	unsigned char* data = stbi_load(filepath.c_str(), &tex_spritesheet_size.x, &tex_spritesheet_size.y, &nrChannels, 0);

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture); // using texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, colortype, tex_spritesheet_size.x, tex_spritesheet_size.y, 0, colortype, GL_UNSIGNED_BYTE, data); // Create texture
	glGenerateMipmap(GL_TEXTURE_2D); // lowers resolution for far away objects
	if (data) { // Error handling
		glTexImage2D(GL_TEXTURE_2D, 0, colortype, tex_spritesheet_size.x, tex_spritesheet_size.y, 0, colortype, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // Frees memory used when loading texture
}

void Spritesheet::free_spritesheet() {
	glDeleteTextures(1, &texture);
	texture = 0;
}

bool Spritesheet::is_loaded() {
	return texture != 0;
}

void Spritesheet::bind_texture() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

int Spritesheet::get_sprite_index(int x_index, int y_index) {
	return y_index * (tex_spritesheet_size.y / tex_sprite_size.y) + x_index * (tex_spritesheet_size.x / tex_sprite_size.x);
}

glm::ivec2 Spritesheet::get_sprite_coords(int index) {
	glm::ivec2 r;
	r.x = index % (tex_spritesheet_size.x / tex_sprite_size.x);
	r.y = index / (tex_spritesheet_size.x / tex_sprite_size.x);

	//std::cout << index << "%" << (tex_spritesheet_size.x / tex_sprite_size.x) <<" -> (" << r.x << ", " << r.y << ")" << std::endl;

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
	return r;
}

glm::vec2 Spritesheet::get_sprite_size() {
	return sprite_size;
}

// Operators
bool operator==(const Spritesheet& lhs, const Spritesheet& rhs) {
	return lhs.texture == rhs.texture &&
		lhs.spritesheet_size == rhs.spritesheet_size &&
		lhs.tex_spritesheet_size == rhs.tex_spritesheet_size &&
		lhs.tex_sprite_size == rhs.tex_sprite_size;
}

bool operator!=(const Spritesheet& lhs, const Spritesheet& rhs) {
	return !(lhs == rhs);
}