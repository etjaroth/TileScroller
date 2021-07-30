#include "Scroller.h"
#include <unordered_set>

void Scroller::load_chunk(glm::ivec2 spot) {

	if (chunks.find(spot) == chunks.end()) {
		std::shared_ptr<Tileset> t = std::make_shared<Tileset>(filepath.c_str(), tilesize, &spatial_hashmap);
		glm::ivec2 start_pos = spot * chunk_size;
		glm::ivec2 end_pos = start_pos + glm::ivec2(chunk_size.x, -chunk_size.y);;

		t->load(start_pos, end_pos);
		chunks.insert({ spot,  t});
	}
}

void Scroller::update_chunks() {
	glm::ivec2 current_hash_pos = hash(get_pos());
	if (current_hash_pos != last_hash_pos) {

		std::unordered_set<glm::ivec2, ivec2_Hash_Function> points;
		last_hash_pos = current_hash_pos;
		glm::ivec2 end_pos = hash(get_bottom_right());

		const int border = 10;
		for (int x = current_hash_pos.x; x < end_pos.x; ++x) {
			for (int y = end_pos.y; y < current_hash_pos.y; ++y) {
				if (x > border || x < -border) {
					continue;
				}
				if (y > border || y < -border) {
					continue;
				}


				points.insert(glm::ivec2(x, y));
			}
		}

		for (std::unordered_set<glm::ivec2, ivec2_Hash_Function>::iterator i = points.begin(); i != points.end(); ++i) {
			glm::ivec2 pos = *i;
			load_chunk(pos);
		}

		for (auto i = chunks.begin(); i != chunks.end();) {
			if (points.find(i->first) == points.end()) {
				i = chunks.erase(i);
			}
			else {
				++i;
			}
		}
	}
}

glm::ivec2 Scroller::hash(glm::ivec2 coord) {
	return coord / glm::ivec2(chunk_size);
}

Scroller::Scroller(glm::vec2 pos, glm::vec2 size, std::string file_path, glm::ivec2 tile_size) : Box(pos, size), spatial_hashmap(glm::vec2(10.0f)) {
	filepath = file_path;
	tilesize = tile_size;

	chunk_size = glm::ivec2(10);

	last_hash_pos = hash(get_pos()) + glm::ivec2(1); // so it will be updated
	update_chunks();
}

void Scroller::render() {
	for (auto i = chunks.begin(); i != chunks.end(); ++i) {
		i->second->render();
	}
}

void Scroller::collide() {
	spatial_hashmap.collide_all();
}

void Scroller::iterate() {
	spatial_hashmap.iterate_all();
}

void Scroller::set_pos(glm::vec2 new_pos) {
	Box::set_pos(new_pos);
	update_chunks();
}

void Scroller::save() {
	for (auto i = chunks.begin(); i != chunks.end(); ++i) {
		i->second->save();
	}
}

void Scroller::write_tile(glm::vec2 pos, int tile_type) {
	glm::ivec2 h_pos;

	enum quadrent { q1 = 1, q2, q3, q4 };
	quadrent quad = q1;

	if (pos.x >= 0) {
		if (pos.y >= 0) { // Q1
			quad = q1;
		}
		else if (pos.y < 0) { // Q4
			quad = q4;
		}
	}
	else if (pos.x <= 0) {
		if (pos.y >= 0) { // Q2
			quad = q2;
		}
		else if (pos.y < 0) { // Q3
			quad = q3;
		}
	}

	switch (quad) {
	case q1:
		h_pos = hash(pos + glm::vec2(0, 0)) + glm::ivec2(0, 1);
		break;
	case q2:
		h_pos = hash(pos + glm::vec2(1, 0)) + glm::ivec2(-1, 1);
		break;
	case q3:
		h_pos = hash(pos + glm::vec2(1, 1)) + glm::ivec2(-1, 0);
		break;
	case q4:
		h_pos = hash(pos + glm::vec2(0, 1.0f));
		break;
	}

	auto i = chunks.find(h_pos);
	if (i != chunks.end()) {
		i->second->write_tile(pos, tile_type);
	}
	else {
		std::cout << "error" << std::endl;
	}
}

void Scroller::insert(std::shared_ptr<Entity> item) {
	spatial_hashmap.insert(item);
}