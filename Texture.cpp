#include "Texture.h"
#include "stb_image.h"

GLuint Texture::currently_bound_texture = 0;
void Texture::auto_bind_texture() {
	if (currently_bound_texture != texture) {
		bind();
	}
}

Texture::Texture() {}

// data and data_size may be default arguments
void Texture::generate_texture(glm::ivec2 size, void* data, unsigned int data_size, GLenum colortype) {
	texture_dimensions = size;

	if (is_loaded()) {
		free();
	}

	int nrChannels;

	glGenTextures(1, &texture);
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, colortype, texture_dimensions.x, texture_dimensions.y, 0, colortype, GL_UNSIGNED_BYTE, data); // Create texture
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::load_texture(std::string filepath, bool flip) {
	if (is_loaded()) {
		free();
	}

	stbi_set_flip_vertically_on_load(flip);

	if (filepath == "button_sprites.png") {
		int i = 0;
		++i;
	}

	int nrChannels;
	const GLenum colortype = GL_RGBA;
	unsigned char* data = stbi_load(filepath.c_str(), &texture_dimensions.x, &texture_dimensions.y, &nrChannels, 0);

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture); // using texture

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, colortype, texture_dimensions.x, texture_dimensions.y, 0, colortype, GL_UNSIGNED_BYTE, data); // Create texture
	//glGenerateMipmap(GL_TEXTURE_2D); // lowers resolution for far away objects
	if (data) { // Error handling
		glTexImage2D(GL_TEXTURE_2D, 0, colortype, texture_dimensions.x, texture_dimensions.y, 0, colortype, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data); // Frees memory used when loading texture
}

bool Texture::is_loaded() {
	return texture != 0;
}

void Texture::free() {
	auto_bind_texture();

	glDeleteTextures(1, &texture);
	texture = 0;
}

void Texture::bind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unbind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

glm::ivec2 Texture::get_size() {
	return texture_dimensions;
}

GLuint Texture::get_texture() {
	return texture;
}

void Texture::set_texture_border_wrap(GLenum style) {
	auto_bind_texture();

	set_texture_border_wrap_x(style);
	set_texture_border_wrap_y(style);
}

void Texture::set_texture_border_wrap_x(GLenum style) {
	auto_bind_texture();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, style);
}

void Texture::set_texture_border_wrap_y(GLenum style) {
	auto_bind_texture();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, style);
}

GLenum Texture::get_texture_border_wrap_x() {
	auto_bind_texture();

	GLint style;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &style);
	return (GLenum)style;
}

GLenum Texture::get_texture_border_wrap_y() {
	auto_bind_texture();

	GLint style;
	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &style);
	return (GLenum)style;
}

// Operators
bool operator==(const Texture& lhs, const Texture& rhs) {
	return lhs.texture == rhs.texture &&
		lhs.texture_dimensions == rhs.texture_dimensions;
}

bool operator!=(const Texture& lhs, const Texture& rhs) {
	return !(lhs == rhs);
}