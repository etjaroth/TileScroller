#pragma once
#include "opengl_libs.h"
#include "Box.h"

class Camera : public Box
{
	glm::mat4 view;
	void calc_view();

public:
	Camera();
	void set_pos(glm::vec2 new_pos) override;
	void set_size(glm::vec2 new_size) override;
	glm::mat4 get_view();
};

