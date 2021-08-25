#include "Spatial_Hashmap.h"

void Spatial_Hashmap::collide_cell(std::unordered_map<glm::ivec2, std::vector<std::shared_ptr<Collision_Box>>, ivec2_Hash_Function>::iterator cell) {
	for (auto i = cell->second.begin(); i != cell->second.end(); ++i) {
		for (auto j = std::next(i); j != cell->second.end(); ++j) {
			(*i)->collide(&(**j));

		}
	}
}

//////////////////////////////////////////////////////////////////////////////

//Spatial_Hashmap::Spatial_Hashmap(glm::vec2 cell_dimensions) : Grid<Collision_Box>(cell_dimensions) {
//	
//}

void Spatial_Hashmap::insert(std::shared_ptr<Collision_Box> box) {
	objects.push_back(box);
	reinsert(box);
}

void Spatial_Hashmap::reinsert(std::shared_ptr<Collision_Box> box) {
	glm::vec2 tl_cell = this->hash_to_cell_size(box->get_top_left());
	glm::vec2 br_cell = this->hash_to_cell_size(box->get_bottom_right());

	for (int x = tl_cell.x; x <= br_cell.x; ++x) {
		for (int y = br_cell.y; y <= tl_cell.y; ++y) {
			insert_at_cell(box, glm::ivec2(x, y));
		}
	}

	
}

void Spatial_Hashmap::collide_all() {
	for (auto i = grid.begin(); i != grid.end(); ++i) {
		collide_cell(i);
	}
}

void Spatial_Hashmap::iterate_all() {
	grid.clear();
	for (std::vector<std::shared_ptr<Collision_Box>>::iterator i = objects.begin(); i != objects.end();) {
		if ((*i)->is_active()) {
			(*i)->iterate();
			reinsert(*i);
			i++;
		}
		else {
			i = objects.erase(i);
		}
	}
}