#include "Text.h"

Text::Text() {

}

Text::Text(std::string text) {
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
		charset;
		char_pos.x = pos.x + offset.x + charset->get_glyph_bearing(c).x * font_size;
		char_pos.y = pos.y + offset.y + (charset->get_glyph_size(c).y - charset->get_glyph_bearing(c).y) * font_size;

		glm::vec2 char_size = glm::vec2(charset->get_glyph_size(c)) * font_size;

		output_string.push_back(std::make_shared<Display_Char>(charset, c, char_pos, char_size));
	}
}

void Text::set_fontsize(float new_size) {
	font_size = new_size;
	set_text(text);
}