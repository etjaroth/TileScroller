#pragma once
#include "Render_Batch.h"
#include "Texture.h"
#include "Shader.h"
#include <unordered_map>

class Display_Charset
{
	Spritesheet charset;
	Render_Batch display;

	struct character {
		char character;

		glm::vec2 sprite_pos;
		glm::vec2 sprite_size;

		glm::ivec2 glyph_size;
		glm::ivec2 glyph_bearing;
		unsigned int advance;

		int y_padding;
	};

	std::unordered_map <char, character> chars;

public:
	Display_Charset();
	void render();

	Render_Batch* get_render_batch_ptr();
	
	glm::vec2 get_sprite_pos(char c);
	glm::vec2 get_sprite_size(char c);

	glm::ivec2 get_glyph_size(char c);
	glm::ivec2 get_glyph_bearing(char c);
	
	unsigned int get_advance(char c);
	int get_y_padding(char c);
};