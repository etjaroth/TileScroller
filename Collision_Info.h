#pragma once
#include "opengl_libs.h"
class Collision_Info
{
	glm::vec2 forced_movement = glm::vec2(0.0f);
	glm::vec2 velocity = glm::vec2(0.0f);
	int collision_case = -1;
	std::string name = "unnamed_object";

public:
	Collision_Info();

	void set_velocity(glm::vec2 velocity);
	glm::vec2 get_velocity();

	void force_move(glm::vec2 move);
	glm::vec2 get_forced_move();

	void set_name(std::string n);
	std::string get_name();

	void set_collision_case(int c) { collision_case = c; }
	std::string get_collision_case_name() {
		std::string r = "error";
		switch (collision_case) {
		case -1:
			r = "no collision";
			break;
		case 0:
			r = "top_left_corner";
			break;
		case 1:
			r = "top_edge";
			break;
		case 2:
			r = "top_right_corner";
			break;
		case 3:
			r = "left_edge";
			break;
		case 4:
			r = "right_edge";
			break;
		case 5:
			r = "bottom_left_corner";
			break;
		case 6:
			r = "bottom_edge";
			break;
		case 7:
			r = "bottom_right_corner";
			break;
		case 8:
			r = "inside";
			break;
		};

		return r;
	}
};