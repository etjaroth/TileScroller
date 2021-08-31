#include "Render_Batch.h"
#include <iostream>

Render_Batch::Render_Batch(Spritesheet* sheet) {
	spritesheet = sheet;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_VERTEX_ARRAY, 0);
}

//////////////////////////////////////////////////////////////////////////////

void Render_Batch::render() {
	if (update_VAO) {
		update_VAO = false;

		// Convert batch to vector
		std::vector<Vertex> render_data;
		std::vector<unsigned int> indices;
		unsigned int base_index = 0;
		for (auto itr = batch.begin(); itr != batch.end(); itr++) {
			// Copy verticies
			for (int i = 0; i < itr->second.data_size; ++i) {
				render_data.push_back(itr->second.data[i]);
			}
			// Copy indices
			for (int i = 0; i < itr->second.indices_size; ++i) {
				indices.push_back(itr->second.indices[i] + base_index);
			}
			base_index += itr->second.data_size;
		}

		// send data to graphics card
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, render_data.size() * sizeof(Vertex), &render_data[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	if (!batch.empty()) {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		spritesheet->bind();
		glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0); // 
		glBindVertexArray(0);
	}
}

Spritesheet* Render_Batch::get_spritesheet() {
	return spritesheet;
}

//////////////////////////////////////////////////////////////////////////////

unsigned int Render_Batch::store_vertices(Vertex* varr, int v_size, unsigned int* indices, int i_size) {
	update_VAO = true;
	batch.insert({ storeage_id , {varr, v_size, indices, i_size} });
	indices_size += i_size;
	return storeage_id++;
}

void Render_Batch::free_vertices(unsigned int space) {
	auto itr = batch.find(space);
	if (itr != batch.end()) {
		indices_size -= itr->second.indices_size;
		batch.erase(space);
	}
}