#include "Text.h"

Text::Text(Display_Charset* char_set) {
	charset = char_set;
}

Text::Text(Display_Charset* char_set, std::string text) {
	charset = char_set;
	set_text(text);
}

Text::Text(Display_Charset* char_set, std::string text, glm::vec2 pos, float size) {
	charset = char_set;
	set_pos(pos);
	font_size = size;
	set_text(text);
}


void Text::set_pos(glm::vec2 new_pos) {
	pos = new_pos;
}

void Text::set_text(std::string new_text) {
	text = new_text;
	output_string.clear();

	glm::vec2 offset = glm::vec2(0.0f);
	for (char& c : text) {
		glm::vec2 char_pos;
		char_pos.x = pos.x + offset.x + charset->get_glyph_bearing(c).x * font_size;
		char_pos.y = pos.y + offset.y + charset->get_glyph_bearing(c).y * font_size;

		std::cout << c << ":\n    Height: " << charset->get_glyph_size(c).y << "  Bearing: " << charset->get_glyph_bearing(c).y << std::endl;

		glm::vec2 char_size = glm::vec2(charset->get_glyph_size(c)) * font_size;

		output_string.push_back(std::make_shared<Display_Char>(charset, c, char_pos, char_size));

		offset.x += (charset->get_advance(c) >> 6) * font_size;
	}
}

void Text::set_fontsize(float new_size) {
	font_size = new_size;
}