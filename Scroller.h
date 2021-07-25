#pragma once
#include "opengl_libs.h"
#include <unordered_map>
#include "glm_Vector_Hash.h"
#include "Tileset.h"
#include "Box.h"
#include <memory>
#include "Spatial_Hashmap.h"

class Scroller : public Box
{
	Spatial_Hashmap spatial_hashmap;
	std::unordered_map<glm::ivec2, std::shared_ptr<Tileset>, ivec2_Hash_Function> chunks;
	glm::ivec2 chunk_size;

	std::string filepath;
	glm::ivec2 tilesize;

	glm::ivec2 last_hash_pos;

	void load_chunk(glm::ivec2 spot);
	void update_chunks();
	

public:
	Scroller(glm::vec2 pos, glm::vec2 size, std::string file_path, glm::ivec2 tile_size);
	void render();
	void collide();
	void iterate();
	void set_pos(glm::vec2 new_pos) override;

	void save();
	void write_tile(glm::vec2 pos, int tile_type);
	glm::ivec2 hash(glm::ivec2 coord);
	void insert(std::shared_ptr<Entity> item);
};

