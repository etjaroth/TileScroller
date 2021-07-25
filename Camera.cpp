#include "Camera.h"


void Camera::calc_view() {
	view = glm::translate(
		glm::scale(glm::mat4(1.0f), glm::vec3(dimensions, 1.0f)), 
		glm::vec3(position, 0.0f));
}


Camera::Camera() : Box(glm::vec2(0.0f), glm::vec2(1.0f)) {
	calc_view();
}

void Camera::set_pos(glm::vec2 new_pos) {
	Box::set_pos(new_pos);
	calc_view();
}

void Camera::set_size(glm::vec2 new_size) {
	Box::set_size(new_size);
	calc_view();
}

glm::mat4 Camera::get_view() {
	return view;
}