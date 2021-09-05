#include "Display_Char.h"

Display_Char::Display_Char(Display_Charset* charset, char c, glm::ivec2 pos, glm::ivec2 size)
	: Sprite(charset->get_render_batch_ptr(), 0, pos, size) {
	display = charset;
	set_char(c);
}

void Display_Char::set_char(char c) {
	character = c;

	set_custom_sprite(display->get_sprite_pos(character), display->get_sprite_size(character));
}

char Display_Char::get_char() {
	return character;
}

glm::ivec2 Display_Char::get_glyph_size(char c) {
	return display->get_glyph_size(character);
}

glm::ivec2 Display_Char::get_glyph_bearing(char c) {
	return display->get_glyph_bearing(character);
}

unsigned int Display_Char::get_advance(char c) {
	return display->get_advance(character);
}