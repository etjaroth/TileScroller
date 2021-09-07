#pragma once
#include "opengl_libs.h"
#include "Display_Charset.h"
#include "Display_Char.h"
#include <vector>

class Text
{
	Display_Charset* charset;

	std::string text = "text";
	std::vector<std::shared_ptr<Display_Char>> output_string;
	glm::vec2 pos;
	float font_size = 1.0f;
	glm::vec4 font_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0);

public:
	Text(Display_Charset* char_set);
	Text(Display_Charset* char_set, std::string text);
	Text(Display_Charset* char_set, std::string text, glm::vec2 pos, float size);
	
	void set_pos(glm::vec2 new_pos);
	void set_text(std::string new_text);
	void set_fontsize(float new_size);
};