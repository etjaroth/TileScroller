#pragma once
#include "opengl_libs.h"
#include "Spritesheet.h"
#include "Vertex.h"

#include <vector>
#include <unordered_map>

struct data {
	Vertex* data; // array of Verticies
	int data_size;

	unsigned int* indices; // array of indices
	int indices_size;
};

class Render_Batch
{
private:
	Spritesheet* spritesheet;
	std::unordered_map<unsigned int, data> batch;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;

	unsigned int storeage_id = 0;
	bool update_VAO = false;
	unsigned int indices_size = 0;

public:
	Render_Batch(Spritesheet* sheet);
	
	void render();
	Spritesheet* get_spritesheet();

	unsigned int store_vertices(Vertex* varr, int v_size, unsigned int* indices, int i_size);
	void free_vertices(unsigned int space);
};