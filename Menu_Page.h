#pragma once
#include "opengl_libs.h"
#include <vector>
#include "Button.h"
#include "Display_Charset.h"
#include "Text.h"

class Menu_Page
{
public:
	enum class Menu_Actions {no_action=0, set_state_to_gameplay};

private:
	Spritesheet* spritesheet;
	Render_Batch render_batch;

	Display_Charset* charset;

	Menu_Actions action = Menu_Actions::no_action;
	std::vector<std::shared_ptr<Text>> text;
	std::vector<std::shared_ptr<Button>> buttons;
	std::vector<Menu_Actions> button_actions;

public:
	Menu_Page(Spritesheet* sheet, Display_Charset* chset);
	void render();
	void add_button(glm::vec2 pos, glm::vec2 size, Menu_Actions action);
	void add_text(glm::vec2 pos, float size, std::string str);

	void press_buttons(glm::vec2 coords, bool press, bool release);
	Menu_Actions get_action();
};

