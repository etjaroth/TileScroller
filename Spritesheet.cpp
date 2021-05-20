#include "Spritesheet.h"
#include "stb_image.h"
#include <iostream>

Spritesheet::Spritesheet() {
	size = glm::ivec2(0.0f);
	sprite_size = glm::ivec2(0.0f);
	size_in_texles = glm::ivec2(0.0f);
	sprite_size_in_texles = glm::ivec2(0.0f);
}

Spritesheet::Spritesheet(const char* filepath, glm::ivec2 sprite_dimensions) {
	sprite_size_in_texles = sprite_dimensions;
	load_spritesheet(filepath);
	size = glm::vec3(1.0f);
	sprite_size = glm::vec2(sprite_size_in_texles) / glm::vec2(size_in_texles);
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
	const GLenum colortype = GL_RGB;
	unsigned char* data = stbi_load(filepath.c_str(), &size_in_texles.x, &size_in_texles.y, &nrChannels, 0);

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture); // using texture

	glTexImage2D(GL_TEXTURE_2D, 0, colortype, size_in_texles.x, size_in_texles.y, 0, colortype, GL_UNSIGNED_BYTE, data); // Create texture
	glGenerateMipmap(GL_TEXTURE_2D); // lowers resolution for far away objects
	if (data) { // Error handling
		glTexImage2D(GL_TEXTURE_2D, 0, colortype, size_in_texles.x, size_in_texles.y, 0, colortype, GL_UNSIGNED_BYTE, data);
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

glm::vec2 Spritesheet::get_sprite_pos(int index) {
	int sprites_per_row = size.x / sprite_size.x;
	int x = index % sprites_per_row;
	int y = index / sprites_per_row;
	return get_sprite_pos(x, y);
}

glm::vec2 Spritesheet::get_sprite_pos(int x_index, int y_index) {
	return sprite_size * glm::ivec2(x_index, y_index);
}

glm::vec2 Spritesheet::get_sprite_size() {
	return sprite_size;
}

// Operators
bool operator==(const Spritesheet& lhs, const Spritesheet& rhs) {
	return lhs.texture == rhs.texture &&
		lhs.size == rhs.size &&
		lhs.sprite_size == rhs.sprite_size &&
		lhs.size_in_texles == rhs.size_in_texles &&
		lhs.sprite_size_in_texles == rhs.sprite_size_in_texles;
}

bool operator!=(const Spritesheet& lhs, const Spritesheet& rhs) {
	return !(lhs == rhs);
}