#include "Tileset.h"
#include "FastNoise.h"
#include "FastNoise.cpp"
#include <random>
#include <fstream>




std::string get_substring(std::string original, std::string start, std::string end) { // exclusive?
	size_t s = original.find(start);
	size_t e;
	if (end == "") {
		e = end.length();
	}
	else {
		e = original.find(end, s);
	}

	bool exclude_s = start != "" && s != std::string::npos;
	bool exclude_e = e != std::string::npos;

	std::string sub = original.substr(s + exclude_s, e - s + !exclude_s - exclude_e);
	return sub;
}



Tileset::Tileset(const char* filepath, glm::ivec2 tilesize, Spatial_Hashmap* spatial_hashmap) : Box(), spritesheet(filepath, tilesize), batch(&spritesheet) {
	collision_map = spatial_hashmap;
	load_tile_properties();
}

void Tileset::insert_tile(glm::ivec2 pos, int sprite) {
	if (pos.x > get_bottom_right().x || pos.x < get_top_left().x || pos.y < get_bottom_right().y || pos.y > get_top_left().y) {
		std::cout << "Tile out of range!" << std::endl;
		return;
	}

	std::unordered_map<glm::ivec2, std::shared_ptr<Entity>, ivec2_Hash_Function>::iterator i = tileset.find(pos);
	if (i != tileset.end()) {
		i->second->deactivate(); // removes it from the spatial hashmap
		tileset.erase(i);
		insert_tile(pos, sprite);
	}
	else {
		std::shared_ptr<Entity> new_tile = std::make_shared<Entity>(&batch, sprite, glm::vec2(pos), glm::vec2(1.0f));
		new_tile->set_depth(0.999f);
		if (tilenames.find(sprite) != tilenames.end()) {
			new_tile->name = tilenames.find(sprite)->second;
		}
		else {
			new_tile->name = "unknown tile: " + std::to_string(sprite);
		}

		tileset.insert({ pos, new_tile });

		if (wallsprites.find(sprite) != wallsprites.end()) {
			new_tile->activate();
			new_tile->set_physics(0.0f, 0.0f, true);
			new_tile->pin();

			new_tile->set_collision_event(1);
			collision_map->insert(new_tile);
		}
	}
}

void Tileset::load_tile_properties(std::string path) {
	std::ifstream file(path);

	if (file.good()) {
		tilenames.clear();
		wallsprites.clear();

		std::string line;
		int sprite_number = 0;
		while (std::getline(file, line)) {
			// Format:
			//save_file << "{(" << i->first.x << ", " << i->first.y << ") : " << i->second->get_sprite_index() << "}" << std::endl;
			std::string tile_name = get_substring(line, "", " ");
			std::string::size_type pos = line.find(' ');
			if (pos == std::string::npos) { // just a name
				tile_name = line;
			}
			else {
				std::string properties = get_substring(line, " ", "");
				while (properties != "") {
					// Read property
					std::string property_name = "";
					if (line.find(' ') != std::string::npos) {
						property_name = properties;
						properties = "";
					}
					else {
						property_name = get_substring(properties, "", " ");
						properties = get_substring(properties, " ", "\0");
					}

					// Set property
					if (property_name == "solid") {
						wallsprites.insert(sprite_number);
					}
				}
			}
			tilenames.insert({ sprite_number, tile_name });

			++sprite_number;
		}
		file.close();
	}
}

void Tileset::render() {
	batch.render();
}

// n is the number that gets inserted into the map, r is a random number, m does nothing
void splash(std::unordered_map<glm::ivec2, int, ivec2_Hash_Function>* map, const glm::ivec2 coord, const int n, int r, const int m, unsigned int cycles) {

	(*map)[coord] = n;

	if (cycles > 0) {
		r = m - m;
		splash(map, coord + glm::ivec2(1, 0), n, r, m, cycles - 1);
		splash(map, coord + glm::ivec2(0, 1), n, r, m, cycles - 1);
		splash(map, coord + glm::ivec2(-1, 0), n, r, m, cycles - 1);
		splash(map, coord + glm::ivec2(0, -1), n, r, m, cycles - 1);
	}
}


void Tileset::save() {
	std::string path = ("gamedata\\savedata\\" + std::to_string((int)position.x) + "x" + std::to_string((int)position.y) + "y.txt");
	save(path);
}

void Tileset::save(std::string path) {
	std::ofstream save_file;
	save_file.open(path);

	if (save_file.is_open()) {
		for (auto i = tileset.begin(); i != tileset.end(); ++i) {
			save_file << "{(" << i->first.x << ", " << i->first.y << ") : " << i->second->get_sprite_index() << "}" << std::endl;
		}
	}
	else {
		std::cout << "error saving " << path << std::endl;
	}

	save_file.close();
}

void Tileset::load(glm::ivec2 start, glm::ivec2 end) {
	set_pos(glm::vec2(start));
	set_size(glm::vec2((end.x - start.x), (start.y - end.y)));

	std::string path = ("gamedata\\savedata\\" + std::to_string((int)position.x) + "x" + std::to_string((int)position.y) + "y.txt");
	std::ifstream file(path);

	if (file.good()) {
		std::string line;
		while (std::getline(file, line)) {
			// Format:
			//save_file << "{(" << i->first.x << ", " << i->first.y << ") : " << i->second->get_sprite_index() << "}" << std::endl;
			int x = std::stoi(get_substring(line, "(", ","));
			int y = std::stoi(get_substring(line, ", ", ")"));;
			int tile_type = std::stoi(get_substring(line, ": ", "}"));

			insert_tile(glm::ivec2(x, y), tile_type);
		}
		file.close();
	}
	else {
		file.close();
		generate_tiles(start, end);
		save(path);
	}
}

void Tileset::generate_tiles(glm::ivec2 start, glm::ivec2 end) {
	float percentage = 0.0f;
	float tiles_done = 0.0f;
	float total_tiles = abs((end.x - start.x) * (end.y - start.y));

	set_pos(glm::vec2(start));
	set_size(glm::vec2((end.x - start.x), (start.y - end.y)));

	/*FastNoise temp_noise(temp_seed);
	temp_noise.SetNoiseType(FastNoise::Perlin);
	temp_noise.SetFrequency(temp_freq);

	FastNoise humid_noise(humid_seed);
	humid_noise.SetNoiseType(FastNoise::Perlin);
	humid_noise.SetFrequency(humid_freq);

	FastNoise water_noise(humid_seed);
	humid_noise.SetNoiseType(FastNoise::Cubic);
	humid_noise.SetFrequency(humid_freq * 0.00001);*/

	std::unordered_map<glm::ivec2, int, ivec2_Hash_Function> map;
	for (int x = start.x; x < end.x; ++x) {
		for (int y = end.y; y < start.y; ++y) {
			/*int temp = (int)(temp_amp * temp_noise.GetNoise(x, y));
			int humid = (int)(humid_amp * humid_noise.GetNoise(x, y));
			int water = (int)(10 * water_noise.GetNoise(x, y));

			tilename t;
			if (water >= 5) {
				t = tilename::water;
			}
			else if (temp >= 2 || humid <= -6) {
				t = tilename::sand;
			}
			else if (temp <= -6) {
				t = tilename::dirt;
			}
			else {
				t = tilename::grass;
			}*/

			//map.insert({ glm::ivec2(x, y), (int)t });
			map.insert({ glm::ivec2(x, y), 1 });
		}
	}

	for (int x = start.x; x < end.x; ++x) {
		for (int y = end.y; y < start.y; ++y) {
			int s = map.find(glm::ivec2(x, y))->second;
			insert_tile(glm::ivec2(x, y), s);
		}
	}
}

void Tileset::write_tile(glm::vec2 pos, int tile_type) {
	if (pos.x >= 0) {
		if (pos.y >= 0) { // Q1
			pos += glm::vec2(0, 1);
		}
		else if (pos.y <= 0) { // Q4
			pos += glm::vec2(0);
		}
	}
	else if (pos.x <= 0) {
		if (pos.y >= 0) { // Q2
			pos += glm::vec2(-1, 1);
		}
		else if (pos.y <= 0) { // Q3
			pos += glm::vec2(-1, 0);
		}
	}

	insert_tile(glm::ivec2(pos), tile_type);
	save();
}