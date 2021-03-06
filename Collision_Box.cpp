#include "Collision_Box.h"

glm::vec2 Collision_Box::get_velocity() {
	return velocity;
}

float Collision_Box::get_elasticity() {
	return elasticity;
}

float Collision_Box::get_mass() {
	if (mass_reciprocal == 0) {
		return INFINITY;
	}
	else {
		return 1.0f / mass_reciprocal;
	}
}

float Collision_Box::get_inverse_mass() {
	return mass_reciprocal;
}

bool Collision_Box::is_solid() {
	return solid;
}

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

Collision_Box::Collision_Box(glm::vec2 pos, glm::vec2 size) : Box(pos, size) {
}

void Collision_Box::iterate() {
	bool collision_happened = false;
	while (has_events()) {
		std::shared_ptr<Collision_Info> cevent = collision_events.front();
		collision_events.pop();

		collision_happened = collision_happened || cevent->is_solid();

		if (!pinned) {
			change_pos(cevent->get_forced_move());

		}
	}

	if (!pinned) {
		change_pos(get_velocity());
	}
}

void Collision_Box::set_pos(glm::vec2 new_pos) {
	Box::set_pos(new_pos);
}

void Collision_Box::set_physics(float e, float m, bool s) {
	elasticity = e;
	mass_reciprocal = (m == 0) ? 0 : 1 / m;
	solid = s;
}

void Collision_Box::set_velocity(glm::vec2 v) {
	velocity = v;
}

void Collision_Box::change_velocity(glm::vec2 v) {
	velocity += v;
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

	// Check if the box missed on a side
	if (my_bottom_right_corner_pos.x < other_top_left_corner_pos.x)
		return false;
	if (my_top_left_corner_pos.x > other_bottom_right_corner_pos.x)
		return false;
	if (my_top_left_corner_pos.y < other_bottom_right_corner_pos.y)
		return false;
	if (my_bottom_right_corner_pos.y > other_top_left_corner_pos.y)
		return false;

	return true;
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
		current_case = collision_cases::inside;
	}

	return current_case;
}

glm::vec2 Collision_Box::collide(Collision_Box* other_box) {
	// Adapted from https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331

	if (this == other_box) {
		return glm::vec2(0.0f);
	}
	if (get_inverse_mass() == 0 && other_box->get_inverse_mass() == 0) {
		return glm::vec2(0.0f);
	}

	// Check if collision happened and calculate collision normal and collision
	bool collision = false;
	glm::vec2 normal; // collision_normal
	float penetration_depth;

	glm::vec2 n = (get_top_left() + get_bottom_right()) / 2.0f - (other_box->get_top_left() + other_box->get_bottom_right()) / 2.0f; // distance between boxes' centres
	//glm::vec2 n = get_pos() - other_box->get_pos();

	// Calculate half extents along x axis for each object
	float ax_extent = (get_bottom_right().x - get_top_left().x) / 2.0f;
	float bx_extent = (other_box->get_bottom_right().x - other_box->get_top_left().x) / 2.0f;

	// Calculate overlap on x axis
	float x_overlap = ax_extent + bx_extent - abs(n.x);

	// SAT test on x axis
	if (x_overlap > 0.0f)
	{
		// Calculate half extents along y axis for each object
		float ay_extent = (get_top_left().y - get_bottom_right().y) / 2.0f;
		float by_extent = (other_box->get_top_left().y - other_box->get_bottom_right().y) / 2.0f;

		// Calculate overlap on y axis
		float y_overlap = ay_extent + by_extent - abs(n.y);

		// SAT test on y axis
		if (y_overlap > 0.0f)
		{
			collision = true;

			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap)
			{
				penetration_depth = x_overlap;

				// Point towards B knowing that n points from A to B
				if (n.x < 0.0f) {
					normal = glm::vec2(-1.0f, 0.0f);
				}
				else {
					normal = glm::vec2(1.0f, 0.0f);
				}

			}
			else
			{
				penetration_depth = y_overlap;

				// Point toward B knowing that n points from A to B
				if (n.y < 0.0f) {
					normal = glm::vec2(0.0f, -1.0f);
				}
				else {
					normal = glm::vec2(0.0f, 1.0f);
				}
			}
		}

	}

	glm::vec2 local_velocity_a = glm::vec2(0.0f);
	glm::vec2 local_velocity_b = glm::vec2(0.0f);
	if (collision) {

		// Resolve collision
		//////////////////////////////////////

		std::shared_ptr<Collision_Info> cinfo_a = other_box->collision_event();
		std::shared_ptr<Collision_Info> cinfo_b = collision_event();

		cinfo_a->set_is_solid(other_box->is_solid());
		cinfo_b->set_is_solid(is_solid());

		if (is_solid()) {
			// Calculate relative velocity
			glm::vec2 relative_velocity = other_box->get_velocity() - get_velocity();

			// Calculate relative velocity in terms of the normal direction
			float velAlongNormal = glm::dot(relative_velocity, normal);

			// Do not resolve if velocities are separating
			if (velAlongNormal < 0.0f) {
				return glm::vec2(0.0f);
			}

			// Calculate restitution
			float elasticity = std::min(get_elasticity(), other_box->get_elasticity());

			// Calculate impulse scalar
			float j = -(1.0f + elasticity) * velAlongNormal;
			j = j / (get_inverse_mass() + other_box->get_inverse_mass());

			// Apply impulse
			glm::vec2 impulse = j * normal;

			/*A.velocity -= 1 / A.mass * impulse;
			B.velocity += 1 / B.mass * impulse;*/

			// Set my velocity
			local_velocity_a = -get_inverse_mass() * impulse;
			//cinfo_a->set_velocity(local_velocity_a);
			change_velocity(local_velocity_a);

			// Set other velocity
			local_velocity_b = other_box->get_inverse_mass() * impulse;
			//cinfo_b->set_velocity(local_velocity_b);
			other_box->change_velocity(local_velocity_b);

			// Correct position (fixes sinking and jittering)
			const float percent = 0.3f; // usually 20% to 80%
			glm::vec2 correction = penetration_depth / (get_inverse_mass() + other_box->get_inverse_mass()) * percent * n;
			//A.position -= A.inv_mass * correction;
			//B.position += B.inv_mass * correction;

			if (glm::length(correction) > 0.001) {
				//change_pos(get_inverse_mass() * correction);
				//other_box->change_pos(-other_box->get_inverse_mass() * correction);

				cinfo_a->force_move(get_inverse_mass() * correction);
				cinfo_b->force_move(-1 * other_box->get_inverse_mass() * correction);
			}
		}

		collision_events.push(cinfo_a);
		other_box->push_collision_event(cinfo_b);
	}
	return local_velocity_a;
}

glm::vec2 Collision_Box::collide(Collision_Box* other_box, glm::vec2 real_previous_move) { // for testing
	//previous_pos = get_pos() - real_previous_move;
	//collide(other_box);
	return collide(other_box);
}

glm::vec2 Collision_Box::get_previous_move() {
	//return this->get_pos() - previous_pos;
	return this->get_pos() - velocity;
}

std::shared_ptr<Collision_Info> Collision_Box::collision_event() {
	std::shared_ptr<Collision_Info> r = std::make_shared<Collision_Info>();

	// if things need to be set, do it here
	r->set_name(name);

	return r;
}

std::shared_ptr<Collision_Info> Collision_Box::collision_event(glm::vec2 exit_move, int i) {
	std::shared_ptr<Collision_Info> r = collision_event();

	r->force_move(exit_move);
	r->set_collision_case(i);

	return r;
}

std::shared_ptr<Collision_Info> Collision_Box::pop_collision_event() {
	if (has_events()) {
		std::shared_ptr<Collision_Info> s = collision_events.front();
		collision_events.pop();
		return s;
	}
	else {
		return std::make_shared<Collision_Info>();
	}
}

void Collision_Box::push_collision_event(std::shared_ptr<Collision_Info> cevent) {
	collision_events.push(cevent);
}

bool Collision_Box::has_events() {
	return !collision_events.empty();
}

void Collision_Box::activate() {
	active = true;
}

void Collision_Box::deactivate() {
	active = false;
}

bool Collision_Box::is_active() {
	return active;
}

void Collision_Box::set_collision_event(int collision_event) {
	//my_collision_event = std::make_shared<Collision_Info>();
	switch (collision_event) {
	case 0:
		break;
	case 1:
		//my_collision_event->make_solid();
		solid = true;
		break;
	};
}