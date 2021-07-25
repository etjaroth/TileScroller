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

	enum class tilename {dirt=0, grass, sand, water};
	
	const int temp_seed = 12345;
	const float temp_freq = 0.00525;
	const float temp_amp = 8.0f;
	
	const int humid_seed = 67890;
	const float humid_freq = 0.0105;
	const float humid_amp = 8.0f;

	void insert_tile(glm::ivec2 pos, int sprite);

public:
	Tileset(const char* filepath, glm::ivec2 tilesize, Spatial_Hashmap* spatial_hashmap);
	void render();

	void save();
	void load(glm::ivec2 start, glm::ivec2 end);

	void save(std::string path);
	void generate_tiles(glm::ivec2 start, glm::ivec2 end);

	void write_tile(glm::vec2 pos, int tile_type);
};

