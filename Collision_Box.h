#pragma once
#include "opengl_libs.h"
#include "Box.h"
#include "Collision_Info.h"
#include <queue>

class Collision_Box: virtual public Box
{
protected:
	bool active = false;
	std::queue<std::shared_ptr<Collision_Info>> collision_events;
	//std::shared_ptr<Collision_Info> my_collision_event;
	
	// Collision information
	glm::vec2 velocity = glm::vec2(0.0f);
	float elasticity = 1.0f;
	float mass_reciprocal = 1.0f;

	// For generating a collision event
	bool solid = false;
	bool pinned = false; // decides if the box can be pushed

	// Functions
	float calc_left_exit_move(Collision_Box* other_box);
	float calc_right_exit_move(Collision_Box* other_box);
	float calc_top_exit_move(Collision_Box* other_box);
	float calc_bottom_exit_move(Collision_Box* other_box);

	bool soft_collide(Collision_Box* other_box);

public:
	std::string name = "unnamed object";

	Collision_Box(glm::vec2 pos, glm::vec2 size);
	Collision_Box(glm::vec2 pos, glm::vec2 size, bool activate) : Collision_Box(pos, size) { active = activate; }
	void iterate();
	
	void set_pos(glm::vec2 new_pos) override;

	// Physics
	void set_physics(float e, float m, bool s);

	void set_velocity(glm::vec2 v);
	void change_velocity(glm::vec2 v);

	glm::vec2 get_velocity();
	float get_elasticity();
	float get_mass();
	float get_inverse_mass();
	bool is_solid();
	
	
	enum class collision_cases { top_left_corner = 0, top_edge, top_right_corner, left_edge, right_edge, bottom_left_corner, bottom_edge, bottom_right_corner, inside };
	collision_cases get_collision_case(Collision_Box* other_box);
	glm::vec2 collide(Collision_Box* other_box);
	glm::vec2 collide(Collision_Box* other_box, glm::vec2 real_previous_move);
	
	glm::vec2 get_previous_move();
	std::shared_ptr<Collision_Info> collision_event();
	std::shared_ptr<Collision_Info> collision_event(glm::vec2 velocity, int i);
	std::shared_ptr<Collision_Info> pop_collision_event();
	bool has_events();

	void activate();
	void deactivate();
	void set_collision_event(int collision_event);
	
	void pin() { pinned = true; mass_reciprocal = 0; }
	void unpin() { pinned = false; }
};