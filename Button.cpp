#include "Button.h"
Button::Button(glm::vec2 pos, glm::vec2 size, Render_Batch* batch) : 
	Box(pos, size), 
	Sprite(batch, 0/* this should be (int)state, but this is always called before state is initilized */, pos, size), 
	state(State::neutral) {

}

void Button::set_state(glm::vec2 mouse_coords) {
	if (state == State::released && input_delay_counter > 0) {
		--input_delay_counter;
	}

	State old_state = state;

	if (get_top_left().x <= mouse_coords.x &&
		get_top_left().y >= mouse_coords.y &&
		get_bottom_right().x >= mouse_coords.x &&
		get_bottom_right().y <= mouse_coords.y) {
		if (button_pressed_this_frame) {
			state = State::pressed;
		}
		else if (button_released && state == State::pressed) {
			state = State::released;
		}
		else {
			state = State::highlight;
		}
	}
	else {
		state = State::neutral;
	}

	button_pressed_this_frame = false;
	button_released = false;

	if ((int)state != (int)old_state) {
		set_sprite((int)state);
	}
}

void Button::check_button_press(bool press, bool release) {
	button_pressed_this_frame = press || button_pressed_this_frame;
	button_released = release || button_released;
}

bool Button::is_pressed() {
	return state == State::pressed;
}

bool Button::is_released() {
	if (input_delay_counter <= 0) { // Switching menus instantly feels bad
		input_delay_counter = input_delay;
	}
	else {
		return false;
	}

	return state == State::released;
}