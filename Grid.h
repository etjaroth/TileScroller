#pragma once
#include "Box.h"
#include <unordered_map>
#include <set>
#include "glm_Vector_Hash.h"

template <typename T>
class Grid 
{
protected:
	std::unordered_map<glm::ivec2, std::vector<std::shared_ptr<T>>, ivec2_Hash_Function> grid;
	glm::vec2 cell_size;

	glm::ivec2 hash_to_cell_size(glm::vec2 vec) { return glm::ivec2(vec / cell_size); }
	
	void insert_at_cell(std::shared_ptr<T> item, glm::ivec2 cell) {
		auto itr = grid.find(cell);
		if (itr == grid.end()) {
			grid.insert({ cell, std::vector<std::shared_ptr<T>>{item} });
		}
		else {
			if (std::find(itr->second.begin(), itr->second.end(), item) == itr->second.end()) { // prevent duplicates
				itr->second.push_back(item);
			}
		}
	}

public:
	Grid(glm::vec2 cell_dimensions) {
		cell_size = cell_dimensions;
	}
};