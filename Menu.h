#pragma once
#include "opengl_libs.h"
#include <vector>
#include "Menu_Page.h"

class Menu
{
	// Mouse coordinates
	glm::vec2 mouse_pos;
	glm::mat4 inverse_model;
	glm::mat4 inverse_view;
	glm::mat4 inverse_projection;

	Spritesheet sheet;

public:
	// State
	enum class Game_State { main_menu = 0, gameplay, paused};

private:
	Game_State state;
	std::vector<Menu_Page> pages;

	// Rendering

public:
	Menu();
	void render();

	// Mouse coordinates
	void set_mouse_pos(glm::vec2 pos);
	void make_inverse_model(glm::mat4 model);
	void make_inverse_view(glm::mat4 view);
	void make_inverse_projection(glm::mat4 projection);
	glm::vec2 get_mouse_world_pos();

	// State
	Game_State get_state();
	void set_state(Game_State s);

	// Input
	void press_buttons(bool press, bool release);
};