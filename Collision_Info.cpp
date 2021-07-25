#include "Collision_Info.h"

Collision_Info::Collision_Info() {

}

void Collision_Info::set_velocity(glm::vec2 velocitY) {
	velocity = velocitY;
}

glm::vec2 Collision_Info::get_velocity() {
	return velocity;
}

void Collision_Info::force_move(glm::vec2 move) {
	forced_movement = move;
}

glm::vec2 Collision_Info::get_forced_move() {
	return forced_movement;
}

void Collision_Info::set_name(std::string n) {
	name = n;
}

std::string Collision_Info::get_name() {
	return name;
}