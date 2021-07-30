#include "Sprite.h"

Sprite::Sprite(Render_Batch* sheet, int sprite, glm::vec2 pos, glm::vec2 size) : Box(pos, size) {
	set_batch(sheet, sprite);
	update_render_square_position();
}


Sprite::Sprite(Render_Batch* sheet, glm::ivec2 sprite, glm::vec2 pos, glm::vec2 size) {
	set_batch(sheet, sprite);
	update_render_square_position();
}


void Sprite::set_pos(glm::vec2 new_pos) {
	Box::set_pos(new_pos);
	this->update_render_square_position();
}

void Sprite::set_size(glm::vec2 new_size) {
	Box::set_size(new_size);
	this->update_render_square_position();
}

void Sprite::set_depth(float d) {
	depth = d;
	this->update_render_square_position();
}

void Sprite::change_depth(float d) {
	depth += d;
	this->update_render_square_position();
}

//////////////////////////////////////////////////////////////////////////////

void Sprite::update_render_square_position() {
	render_square[0].position = glm::vec3(get_top_left(), depth);
	render_square[1].position = glm::vec3(get_top_right(), depth);
	render_square[2].position = glm::vec3(get_bottom_left(), depth);
	render_square[3].position = glm::vec3(get_bottom_right(), depth);

	spritesheet->refresh_vertices();
}

void Sprite::update_indices() {
	for (int i = 0; i < 6; ++i) {
		element_indicies[i] += 4 * storeage_token;
	}

	spritesheet->refresh_vertices();
}


int Sprite::get_sprite_index() {
	return sprite_index;
}


void Sprite::set_batch(Render_Batch* batch, int index) {
	spritesheet = batch;
	storeage_token = batch->store_vertices(render_square, 4, element_indicies, 6);
	update_indices();
	set_sprite(index);
}

void Sprite::set_batch(Render_Batch* batch, glm::ivec2 index) {
	spritesheet = batch;
	storeage_token = batch->store_vertices(render_square, 4, element_indicies, 6);
	update_indices();
	set_sprite(index.x, index.y);
}

void Sprite::set_sprite(int index) {
	sprite_index = index;

	sprite_coords = spritesheet->get_spritesheet()->get_sprite_pos(index);
	sprite_size = spritesheet->get_spritesheet()->get_sprite_size();

	render_square[0].tex_coords = spritesheet->get_spritesheet()->get_sprite_top_left(index);
	render_square[1].tex_coords = spritesheet->get_spritesheet()->get_sprite_top_right(index);
	render_square[2].tex_coords = spritesheet->get_spritesheet()->get_sprite_bottom_left(index);
	render_square[3].tex_coords = spritesheet->get_spritesheet()->get_sprite_bottom_right(index);

 	spritesheet->refresh_vertices();
}

void Sprite::set_sprite(int x_index, int y_index) {
	sprite_index = spritesheet->get_spritesheet()->get_sprite_index(x_index, y_index);
	sprite_coords = spritesheet->get_spritesheet()->get_sprite_pos(x_index, y_index);
	sprite_size = spritesheet->get_spritesheet()->get_sprite_size();

	render_square[0].tex_coords = sprite_coords;
	render_square[1].tex_coords = sprite_coords + glm::vec2(sprite_size.x, 0.0f);
	render_square[2].tex_coords = sprite_coords + glm::vec2(0.0f, -sprite_size.y);
	render_square[3].tex_coords = sprite_coords + glm::vec2(sprite_size.x, -sprite_size.y);

	spritesheet->refresh_vertices();
}