#include "Collision_Box.h"

float Collision_Box::calc_left_exit_move(Collision_Box* other_box) {
	return other_box->get_top_left().x - get_bottom_right().x;
}

float Collision_Box::calc_right_exit_move(Collision_Box* other_box) {
	return other_box->get_bottom_right().x - get_top_left().x;
}

float Collision_Box::calc_top_exit_move(Collision_Box* other_box) {
	return other_box->get_top_left().y - get_bottom_right().y;
}

float Collision_Box::calc_bottom_exit_move(Collision_Box* other_box) {
	return other_box->get_bottom_right().y - get_top_left().y;
}


Collision_Box::Collision_Box() {
	position = glm::vec2(0.0f);
	dimensions = glm::vec2(1.0f);
}

Collision_Box::Collision_Box(glm::vec2 pos, glm::vec2 size) {
	position = pos;
	dimensions = size;
}


bool Collision_Box::soft_collide(Collision_Box* other_box) {
	if (this == other_box) {
		return false;
	}

	// Start
	glm::vec2 my_top_left_corner_pos = get_top_left();
	glm::vec2 other_top_left_corner_pos = other_box->get_top_left();

	// End
	glm::vec2 my_bottom_right_corner_pos = get_bottom_right();
	glm::vec2 other_bottom_right_corner_pos = other_box->get_bottom_right();

	bool x_collision = true;
	if (my_bottom_right_corner_pos.x < other_top_left_corner_pos.x) { // other box is too far right
		x_collision = false;
	}
	else if (other_bottom_right_corner_pos.x < my_top_left_corner_pos.x) { // other box is too far left
		x_collision = false;
	}

	bool y_collision = true;
	if (my_bottom_right_corner_pos.y > other_top_left_corner_pos.y) { // other box is too far up
		y_collision = false;
	}
	else if (other_bottom_right_corner_pos.y > my_top_left_corner_pos.y) { // other box is too far down
		y_collision = false;
	}

	return x_collision && y_collision;
}

// Determines which corner/edge of the other box this box is touching
Collision_Box::collision_cases Collision_Box::get_collision_case(Collision_Box* other_box) {
	
	// Get descriptions of the boxes
	glm::vec2 my_top_left_corner_pos = get_top_left();
	glm::vec2 other_top_left_corner_pos = other_box->get_top_left();
	glm::vec2 my_bottom_right_corner_pos = get_bottom_right();
	glm::vec2 other_bottom_right_corner_pos = other_box->get_bottom_right();

	//////////////////////////////////////////////////////////////////////////

	// Note that the case refers to the part of the other box that this box is touching
	collision_cases current_case;
	if (my_top_left_corner_pos.y > other_top_left_corner_pos.y &&
		((my_top_left_corner_pos.x <= other_top_left_corner_pos.x && my_bottom_right_corner_pos.x >= other_bottom_right_corner_pos.x) ||
			(my_top_left_corner_pos.x > other_top_left_corner_pos.x && my_bottom_right_corner_pos.x < other_bottom_right_corner_pos.x))) {
		current_case = collision_cases::top_edge;
	}
	else if (my_top_left_corner_pos.x < other_top_left_corner_pos.x &&
		((my_top_left_corner_pos.y >= other_top_left_corner_pos.y && my_bottom_right_corner_pos.y <= other_bottom_right_corner_pos.y) ||
			(my_top_left_corner_pos.y < other_top_left_corner_pos.y && my_bottom_right_corner_pos.y > other_bottom_right_corner_pos.y))) {
		current_case = collision_cases::left_edge;
	}
	else if (my_bottom_right_corner_pos.x > other_bottom_right_corner_pos.x &&
		((my_top_left_corner_pos.y >= other_top_left_corner_pos.y && my_bottom_right_corner_pos.y <= other_bottom_right_corner_pos.y)
			|| (my_top_left_corner_pos.y < other_top_left_corner_pos.y && my_bottom_right_corner_pos.y > other_bottom_right_corner_pos.y))) {
		current_case = collision_cases::right_edge;
	}
	else if (my_top_left_corner_pos.x < other_top_left_corner_pos.x && my_top_left_corner_pos.y > other_top_left_corner_pos.y && my_bottom_right_corner_pos.y > other_bottom_right_corner_pos.y) {
		current_case = collision_cases::top_left_corner;
	}
	else if (my_top_left_corner_pos.y > other_top_left_corner_pos.y && my_top_left_corner_pos.x > other_top_left_corner_pos.x && my_bottom_right_corner_pos.x > other_bottom_right_corner_pos.x) {
		current_case = collision_cases::top_right_corner;
	}
	else if (my_top_left_corner_pos.y < other_top_left_corner_pos.y &&
		((my_top_left_corner_pos.x <= other_top_left_corner_pos.x && my_bottom_right_corner_pos.x >= other_bottom_right_corner_pos.x) ||
			(my_top_left_corner_pos.x > other_top_left_corner_pos.x && my_bottom_right_corner_pos.x < other_bottom_right_corner_pos.x))) {
		current_case = collision_cases::bottom_edge;
	}
	else if (my_top_left_corner_pos.x < other_top_left_corner_pos.x && my_top_left_corner_pos.y < other_top_left_corner_pos.y && my_bottom_right_corner_pos.y < other_bottom_right_corner_pos.y) {
		current_case = collision_cases::bottom_left_corner;
	}
	
	else if (my_top_left_corner_pos.y < other_top_left_corner_pos.y && my_top_left_corner_pos.x > other_top_left_corner_pos.x && my_bottom_right_corner_pos.y < other_bottom_right_corner_pos.y) {
		current_case = collision_cases::bottom_right_corner;
	}
	else {
		current_case = collision_cases::inside; // uh oh
	}

	return current_case;
}

glm::vec2 Collision_Box::hard_collide(Collision_Box* other_box, glm::vec2 previous_move) {
	glm::vec2 exit_move = glm::vec2(0.0f);
	if (soft_collide(other_box)) {

		//////////////////////////////////////////////////////////////////////

		enum class axis {x=0, y, both};
		axis movement_axis = axis::both;
		if (abs(previous_move.x) < abs(previous_move.y)) {
			movement_axis = axis::y;
		}
		else if (abs(previous_move.x) > abs(previous_move.y)) {
			movement_axis = axis::x;
		}

		//////////////////////////////////////////////////////////////////////

		collision_cases current_case = get_collision_case(other_box);
		switch (current_case) {
		case collision_cases::top_left_corner:
			if (movement_axis == axis::x) {
				exit_move.x = calc_left_exit_move(other_box);
			}
			else if (movement_axis == axis::y) {
				exit_move.y = calc_top_exit_move(other_box);
			}
			else {
				exit_move.x = calc_left_exit_move(other_box);
				exit_move.y = calc_top_exit_move(other_box);
			}
			break;
			
		
		case collision_cases::top_edge:
			exit_move.y = calc_top_exit_move(other_box);
			break;
		
		case collision_cases::top_right_corner:
			if (movement_axis == axis::x) {
				exit_move.x = calc_right_exit_move(other_box);
			}
			else if (movement_axis == axis::y) {
				exit_move.y = calc_top_exit_move(other_box);
			}
			else {
				exit_move.x = calc_right_exit_move(other_box);
				exit_move.y = calc_top_exit_move(other_box);
			}
			break;
		
		case collision_cases::left_edge:
			exit_move.x = calc_left_exit_move(other_box);
			break;
		
		case collision_cases::right_edge:
			exit_move.x = calc_right_exit_move(other_box);
			break;
		
		case collision_cases::bottom_left_corner:
			if (movement_axis == axis::x) {
				exit_move.x = calc_left_exit_move(other_box);
			}
			else if (movement_axis == axis::y) {
				exit_move.y = calc_bottom_exit_move(other_box);
			}
			else {
				exit_move.x = calc_left_exit_move(other_box);
				exit_move.y = calc_bottom_exit_move(other_box);
			}
			break;
		
		case collision_cases::bottom_edge:
			exit_move.y = calc_bottom_exit_move(other_box);
			break;
		
		case collision_cases::bottom_right_corner:
			if (movement_axis == axis::x) {
				exit_move.x = calc_right_exit_move(other_box);
			}
			else if (movement_axis == axis::y) {
				exit_move.y = calc_bottom_exit_move(other_box);
			}
			else {
				exit_move.x = calc_right_exit_move(other_box);
				exit_move.y = calc_bottom_exit_move(other_box);
			}
			break;
		
		case collision_cases::inside: // uh oh
			break;
		};
	}

	return exit_move;
}