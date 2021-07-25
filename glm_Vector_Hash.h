#pragma once
#include "opengl_libs.h"

struct vec2_Hash_Function {
	std::size_t operator() (const glm::vec2& vec) const {
		std::size_t h = std::hash<float>()(vec.x);// +0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= std::hash<float>()(vec.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
		return h;
	}
};

struct ivec2_Hash_Function {
	std::size_t operator() (const glm::ivec2& vec) const {
		std::size_t h = std::hash<int>()(vec.x);// +0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= std::hash<int>()(vec.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
		return h;
	}
};

struct vec3_Hash_Function {
	std::size_t operator() (const glm::vec3& vec) const {
		std::size_t h = std::hash<float>()(vec.x);// +0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= std::hash<float>()(vec.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= std::hash<float>()(vec.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
		return h;
	}
};

struct ivec3_Hash_Function {
	std::size_t operator() (const glm::ivec3& vec) const {
		std::size_t h = std::hash<int>()(vec.x);// +0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= std::hash<int>()(vec.y) + 0x9e3779b9 + (h << 6) + (h >> 2);
		h ^= std::hash<int>()(vec.z) + 0x9e3779b9 + (h << 6) + (h >> 2);
		return h;
	}
};