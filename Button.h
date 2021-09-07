#pragma once
#include "Box.h"
#include "Sprite.h"
#include "Render_Batch.h"
class Button : virtual public Sprite
{
	enum class State {neutral=0, highlight=1, pressed=2, released=2};
	State state;
	bool button_pressed_this_frame = false;
	bool button_released = false;
	const int input_delay = 10;
	int input_delay_counter = 10;

public:
	Button(glm::vec2 pos, glm::vec2 size, Render_Batch* batch);
	void check_button_press(bool press, bool release);
	void set_state(glm::vec2 mouse_coords);
	bool is_pressed();
	bool is_released();
};