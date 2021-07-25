#pragma once
#include "opengl_libs.h"
#include "Collision_Box.h"
#include <unordered_map>
#include <vector>
#include "glm_Vector_Hash.h"
#include <memory>
#include "Grid.h"
//#include "Grid.cpp"
#include <set>

class Spatial_Hashmap : public Grid<Collision_Box>
{
	std::set<std::shared_ptr<Collision_Box>> insert_queue;
	void collide_cell(std::unordered_map<glm::ivec2, std::vector<std::shared_ptr<Collision_Box>>, ivec2_Hash_Function>::iterator cell);

public:
	Spatial_Hashmap(glm::vec2 cell_dimensions) : Grid<Collision_Box>(cell_dimensions) {}
	void insert(std::shared_ptr<Collision_Box> box);
	void iterate_all();
	void collide_all();
};