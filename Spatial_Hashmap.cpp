#include "Spatial_Hashmap.h"

void Spatial_Hashmap::collide_cell(std::unordered_map<glm::ivec2, std::vector<std::shared_ptr<Collision_Box>>, ivec2_Hash_Function>::iterator cell) {
	for (auto i = cell->second.begin(); i != cell->second.end(); ++i) {
		for (auto j = std::next(i); j != cell->second.end(); ++j) {
			(*i)->collide(&(**j));
			//(*j)->collide(&(**i));
			//if ((*j)->collide(&(**i)) != glm::vec2(0.0f)) {
				//(*j)->iterate();
			//}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////

//Spatial_Hashmap::Spatial_Hashmap(glm::vec2 cell_dimensions) : Grid<Collision_Box>(cell_dimensions) {
//	
//}

void Spatial_Hashmap::insert(std::shared_ptr<Collision_Box> box) {
	int i = 0;
	glm::vec2 tl_cell = this->hash_to_cell_size(box->get_top_left());
	glm::vec2 br_cell = this->hash_to_cell_size(box->get_bottom_right());

	for (int x = tl_cell.x; x <= br_cell.x; ++x) {
		for (int y = br_cell.y; y <= tl_cell.y; ++y) {
			insert_at_cell(box, glm::ivec2(x, y));
			++i;
		}
	}

	
}

void Spatial_Hashmap::collide_all() {
	for (std::unordered_map<glm::ivec2, std::vector<std::shared_ptr<Collision_Box>>, vec2_Hash_Function, ivec2_Hash_Function>::iterator i = grid.begin(); i != grid.end(); ++i) {
		collide_cell(i);
	}
}

void Spatial_Hashmap::iterate_all() {
	std::set<std::shared_ptr<Collision_Box>> iterated_items; // Items can be in more than one cell, so we need to prevent them from being iterated twice


	for (auto cell_itr = grid.begin(); cell_itr != grid.end(); ++cell_itr) {
		for (auto item_itr = cell_itr->second.begin(); item_itr != cell_itr->second.end();) {
			

			glm::vec2 tl_cell = this->hash_to_cell_size((*item_itr)->get_top_left());
			glm::vec2 br_cell = this->hash_to_cell_size((*item_itr)->get_bottom_right());

			if (br_cell.x < cell_itr->first.x || tl_cell.x > cell_itr->first.x || tl_cell.y < cell_itr->first.y || br_cell.y > cell_itr->first.y) { // if item is outside of its cell
				if (insert_queue.find(*item_itr) == insert_queue.end()) {
					insert_queue.insert(*item_itr);
				}
				item_itr = cell_itr->second.erase(item_itr);
			}
			else if (!(*item_itr)->is_active()) { // only active items need to be stored
				item_itr = cell_itr->second.erase(item_itr);
			}
			else {
				if (iterated_items.find((*item_itr)) == iterated_items.end()) {
					(*item_itr)->iterate();
					iterated_items.insert((*item_itr));
				}
				
				++item_itr;
			}
		}
	}

	for (auto i = insert_queue.begin(); i != insert_queue.end(); ++i) {
		insert(*i);
	}
}