#include "Sprite.h"

Sprite::Sprite(Render_Batch* sheet, int sprite) {
	Box::Box();
	update_render_square_position();
	set_batch(sheet, sprite);
}

Sprite::Sprite(Render_Batch* sheet, int sprite, glm::vec2 pos, glm::vec2 size) {
	Box::Box(pos, size);
	update_render_square_position();
	set_batch(sheet, sprite);
}


void Sprite::set_pos(glm::vec2 new_pos) {
	Box::set_pos(new_pos);
	update_render_square_position();
}

void Sprite::set_size(glm::vec2 new_size) {
	Box::set_size(new_size);
	update_render_square_position();
}

//////////////////////////////////////////////////////////////////////////////

void Sprite::update_render_square_position() {
	render_square[0].position = get_top_left();
	render_square[1].position = get_top_right();
	render_square[2].position = get_bottom_left();
	render_square[3].position = get_bottom_right();
}

void Sprite::set_batch(Render_Batch* batch, int index) {
	spritesheet = batch;
	batch->store_vertices(render_square, 4, element_indicies, 6);
	set_sprite(index);
}

void Sprite::set_sprite(int index) {
	sprite_coords = spritesheet->get_spritesheet()->get_sprite_pos(index);
	sprite_size = spritesheet->get_spritesheet()->get_sprite_size();
	
	render_square[0].tex_coords = sprite_coords;
	render_square[1].tex_coords = sprite_coords + glm::vec2(sprite_size.x, 0.0f);
	render_square[2].tex_coords = sprite_coords + glm::vec2(0.0f, -sprite_size.y);
	render_square[3].tex_coords = sprite_coords + glm::vec2(sprite_size.x, -sprite_size.y);
}

void Sprite::set_sprite(int x_index, int y_index) {
	sprite_coords = spritesheet->get_spritesheet()->get_sprite_pos(x_index, y_index);
	sprite_size = spritesheet->get_spritesheet()->get_sprite_size();

	render_square[0].tex_coords = sprite_coords;
	render_square[1].tex_coords = sprite_coords + glm::vec2(sprite_size.x, 0.0f);
	render_square[2].tex_coords = sprite_coords + glm::vec2(0.0f, -sprite_size.y);
	render_square[3].tex_coords = sprite_coords + glm::vec2(sprite_size.x, -sprite_size.y);
}