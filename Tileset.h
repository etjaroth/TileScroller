#pragma once
#include "Box.h"
#include "Spatial_Hashmap.h"
#include "Render_Batch.h"
#include <unordered_map>
#include "Entity.h"
#include <memory>
#include "glm_Vector_Hash.h"
#include "Spritesheet.h"

class Tileset : public Box
{
	std::unordered_map<glm::ivec2, std::shared_ptr<Entity>, ivec2_Hash_Function> tileset;
	Spatial_Hashmap* collision_map;
	Spritesheet spritesheet;
	Render_Batch batch;

	std::unordered_map<int, std::string> tilenames = { {0, "dirt"}, {1, "grass"}, {2, "sand"}, {3, "water"},
	{4, "road"}, {5, "brown"}, {6, "grey"}, {7, "wood floor"},
	{8, "tree leaves"}, {9, ""}, {10, ""}, {11, ""},
	{12, ""}, {13, ""}, {14, ""}, {15, ""} };
	std::set<int> wallsprites = { 3, 5, 6, 8 };
	
	const int temp_seed = 12345;
	const float temp_freq = 0.00525;
	const float temp_amp = 8.0f;
	
	const int humid_seed = 67890;
	const float humid_freq = 0.0105;
	const float humid_amp = 8.0f;

	void insert_tile(glm::ivec2 pos, int sprite);
	void load_tile_properties(std::string path="gamedata\\tile_properties.txt");

public:
	Tileset(const char* filepath, glm::ivec2 tilesize, Spatial_Hashmap* spatial_hashmap);
	void render();

	void save();
	void load(glm::ivec2 start, glm::ivec2 end);

	void save(std::string path);
	void generate_tiles(glm::ivec2 start, glm::ivec2 end);

	void write_tile(glm::vec2 pos, int tile_type);
};

