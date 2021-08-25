#include "Menu_Page.h"

Menu_Page::Menu_Page(Spritesheet* sheet) : render_batch(sheet) {
	spritesheet = sheet;
}

void Menu_Page::render() {
	render_batch.render();
}

void Menu_Page::add_button(glm::vec2 pos, glm::vec2 size, Menu_Actions action) {
	buttons.push_back(std::make_shared<Button>(pos, size, &render_batch));
	button_actions.push_back(action);
}

void Menu_Page::press_buttons(glm::vec2 coords, bool press, bool release) {
	for (auto button = buttons.begin(); button != buttons.end(); button++) {
		
		(*button)->check_button_press(press, release);
		
		(*button)->set_state(coords);
		if ((*button)->is_released()) {
			action = button_actions[button - buttons.begin()];
		}
	}
}

Menu_Page::Menu_Actions Menu_Page::get_action() {
	return action;
}