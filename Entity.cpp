#include "Entity.h"
Entity::Entity(Render_Batch* sheet, int sprite, glm::vec2 pos, glm::vec2 size) : Box(pos, size), Collision_Box(pos, size), Sprite(sheet, sprite, pos, size) {
}

Entity::Entity(Render_Batch* sheet, glm::ivec2 sprite, glm::vec2 pos, glm::vec2 size) : Box(pos, size), Collision_Box(pos, size), Sprite(sheet, sprite, pos, size) {
}

void Entity::set_pos(glm::vec2 new_pos) {
	//previous_pos = get_pos();
	Collision_Box::set_pos(new_pos); // update previous pos (must be first)
	Sprite::set_pos(new_pos); // update sprite
}