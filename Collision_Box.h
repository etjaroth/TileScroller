#pragma once
#include "opengl_libs.h"
#include "Box.h"

class Collision_Box: public Box
{
private:
	float calc_left_exit_move(Collision_Box* other_box);
	float calc_right_exit_move(Collision_Box* other_box);
	float calc_top_exit_move(Collision_Box* other_box);
	float calc_bottom_exit_move(Collision_Box* other_box);

public:
	Collision_Box();
	Collision_Box(glm::vec2 pos, glm::vec2 size);

	bool soft_collide(Collision_Box *other_box);
	
	enum class collision_cases { top_left_corner = 0, top_edge, top_right_corner, left_edge, right_edge, bottom_left_corner, bottom_edge, bottom_right_corner, inside };
	collision_cases get_collision_case(Collision_Box* other_box);
	glm::vec2 hard_collide(Collision_Box* other_box, glm::vec2 previous_move);
};