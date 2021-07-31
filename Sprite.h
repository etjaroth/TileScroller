#pragma once
#include "opengl_libs.h"
#include "Box.h"
#include "Render_Batch.h"
#include "Vertex.h"
#include <memory>

class Sprite : virtual public Box
{
protected:

	// Render information
	Render_Batch* spritesheet;
	glm::vec2 sprite_coords;
	glm::vec2 sprite_size;

	Vertex render_square[4]; // top left, top right, bottom left, bottom right
	GLuint element_indicies[6] = { 1, 2, 3, 0, 2, 1 };
	float depth = 0.0f;

	unsigned int storeage_token;
	int sprite_index;

//protected:
	void update_render_square_position();

public:
	Sprite(Render_Batch* sheet, int sprite, glm::vec2 pos, glm::vec2 size);
	Sprite(Render_Batch* sheet, glm::ivec2 sprite, glm::vec2 pos, glm::vec2 size);
	~Sprite();

	void set_pos(glm::vec2 new_pos) override;
	void set_size(glm::vec2 new_size) override;
	void set_depth(float d);
	void change_depth(float d);

	int get_sprite_index();

	void set_batch(Render_Batch* batch, int index);
	void set_batch(Render_Batch* batch, glm::ivec2 index);
	void set_sprite(int index);
	void set_sprite(int x_index, int y_index);
};