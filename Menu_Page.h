#pragma once
#include "opengl_libs.h"
#include <vector>
#include "Button.h"

class Menu_Page
{
public:
	enum class Menu_Actions {no_action=0, set_state_to_gameplay};

private:
	Spritesheet* spritesheet;
	Render_Batch render_batch;

	Menu_Actions action = Menu_Actions::no_action;
	std::vector<std::shared_ptr<Button>> buttons;
	std::vector<Menu_Actions> button_actions;

public:
	Menu_Page(Spritesheet* sheet);
	void render();
	void add_button(glm::vec2 pos, glm::vec2 size, Menu_Actions action);

	void press_buttons(glm::vec2 coords, bool press, bool release);
	Menu_Actions get_action();
};

