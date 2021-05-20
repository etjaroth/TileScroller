#include "Box.h"

Box::Box() {
	position = glm::vec2(0.0f);
	dimensions = glm::vec2(1.0f);
}

Box::Box(glm::vec2 pos, glm::vec2 size) {
	position = pos;
	dimensions = size;
}



void Box::set_pos(glm::vec2 new_pos) {
	position = new_pos;
}

void Box::set_size(glm::vec2 new_dimensions) {
	dimensions = new_dimensions;
}



void Box::change_pos(glm::vec2 pos_change) {
	this->set_pos(this->get_pos() + pos_change);
}

void Box::change_size(glm::vec2 dimensions_change) {
	this->set_size(this->get_size() + dimensions_change);
}



glm::vec2 Box::get_pos() {
	return position;
}

glm::vec2 Box::get_size() {
	return dimensions;
}

glm::vec2 Box::get_top_left() {
	return position;
}

glm::vec2 Box::get_top_right() {
	return position + glm::vec2(dimensions.x, 0.0f);
}

glm::vec2 Box::get_bottom_left() {
	return position + glm::vec2(0.0f, -dimensions.y);
}

glm::vec2 Box::get_bottom_right() {
	return position + glm::vec2(dimensions.x, -dimensions.y);
}


bool operator==(const Box& lhs, const Box& rhs) {
	return lhs.position == rhs.position && lhs.dimensions == rhs.dimensions;
}

bool operator!=(const Box& lhs, const Box& rhs) {
	return !(lhs == rhs);
}