#pragma once
#include "opengl_libs.h"

class Box
{
protected:
	glm::vec2 position;
	glm::vec2 dimensions;

public:
	Box();
	Box(glm::vec2 pos, glm::vec2 size);

	virtual void set_pos(glm::vec2 new_pos);
	virtual void set_size(glm::vec2 new_dimensions);

	virtual void change_pos(glm::vec2 pos_change);
	virtual void change_size(glm::vec2 dimensions_change);

	glm::vec2 get_pos();
	glm::vec2 get_size();

	glm::vec2 get_top_left();
	glm::vec2 get_top_right();
	glm::vec2 get_bottom_left();
	glm::vec2 get_bottom_right();

	// Operators
	friend bool operator==(const Box& lhs, const Box& rhs);
	friend bool operator!=(const Box& lhs, const Box& rhs);
};