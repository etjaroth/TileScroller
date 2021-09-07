#include "Menu.h"
Menu::Menu() : sheet("gamedata\\spritesheets\\button_sprites.png", glm::ivec2(16)) {
	inverse_model = glm::mat4(1.0f);
	inverse_view = glm::mat4(1.0f);
	inverse_projection = glm::mat4(1.0f);

	set_state(Game_State::main_menu);

	// Create menu pages
		// Main Menu
	pages.push_back({ &sheet, &charset });
	pages[0].add_button(glm::vec2(-0.75f, 0.75f), glm::vec2(1.5f, 1.0f), Menu_Page::Menu_Actions::set_state_to_gameplay);
	pages[0].add_text(glm::vec2(-0.5f, 0.2f), 0.01f, "Start");

	press_buttons(false, false); // set sprites so nothing looks weird if the mouse starts off screen
}

void Menu::render() {
	if ((int)state < pages.size()) {
		pages[(int)state].render();
	}
}

// Mouse coordinates

void Menu::set_mouse_pos(glm::vec2 pos) {
	mouse_pos = pos;
}

void Menu::make_inverse_model(glm::mat4 model) {
	inverse_model = glm::inverse(model);
}

void Menu::make_inverse_view(glm::mat4 view) {
	inverse_view = glm::inverse(view);
}

void Menu::make_inverse_projection(glm::mat4 projection) {
	inverse_projection = glm::inverse(projection);
}

glm::vec2 Menu::get_mouse_world_pos() {
	glm::vec2 mouse_world_pos = glm::vec2(inverse_model * inverse_view * inverse_projection * glm::vec4(mouse_pos, 0.0f, 1.0f));
	return mouse_world_pos;
}


// State
Menu::Game_State Menu::get_state() {
	return state;
}

void Menu::set_state(Menu::Game_State s) {
	state = s;
}


// Input
void Menu::press_buttons(bool press, bool release) {
	if ((int)state < pages.size()) {
		pages[(int)state].press_buttons(mouse_pos, press, release);

		switch (pages[(int)state].get_action()) {
		case Menu_Page::Menu_Actions::no_action:
			break;
		case Menu_Page::Menu_Actions::set_state_to_gameplay:
			state = Game_State::gameplay;
			break;
		}
	}
}