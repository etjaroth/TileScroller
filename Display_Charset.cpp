#include "Display_Charset.h"
#include "Display_Char.h"
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H


Display_Charset::Display_Charset() : charset(), display(&charset)
{
	FT_Library ft;

	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		exit(-1);
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, "C:\\Windows\\Fonts\\arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		exit(-1);
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// load first 128 characters of ASCII set
		std::vector<std::vector<unsigned char>> vector2d;
		glm::ivec2 texture_size = glm::ivec2(0);
		for (unsigned char c = 0; c < 128; c++) {
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph '" << c << "'" << std::endl;
				continue;
			}

			glm::ivec2 glyph_size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);

			// Expand texture if needed
			if (glyph_size.y > texture_size.y) {
				unsigned int diffrence = glyph_size.y - texture_size.y;
				for (int i = 0; i < diffrence; ++i) {
					vector2d.push_back(std::vector<unsigned char>(texture_size.x, 0));
				}
				texture_size.y = glyph_size.y;
			}

			// Read glyph to texture
			for (int y = 0; y < face->glyph->bitmap.rows; ++y) {
				for (int x = 0; x < face->glyph->bitmap.width; ++x) {
					int flip_glyph = vector2d.size() - 1 - y;
					vector2d[flip_glyph].push_back((face->glyph->bitmap.buffer)[y * face->glyph->bitmap.width + x]); // treat as 2d array
				}
			}

			// Create the pixels above the glyph
			if (glyph_size.y < texture_size.y) {
				for (int y = glyph_size.y; y < texture_size.y; ++y) {
					for (int x = 0; x < glyph_size.x; ++x) {
						int flip_glyph = vector2d.size() - 1 - y;
						vector2d[flip_glyph].push_back(0);
					}
				}
			}

			texture_size.x += glyph_size.x;
		}

		//std::reverse(vector2d.begin(), vector2d.end());

		// read 2D vector into 1D vector
		int hhhh = texture_size.x * texture_size.y * 4;
		std::vector<unsigned char> vector1d(hhhh); // 4 is for RGBA
		for (int y = 0; y < vector2d.size(); ++y) {
			for (int x = 0; x < vector2d[y].size(); ++x) {
				const unsigned char greyscale_value = vector2d[y][x];
				vector1d[(x + (y * texture_size.x)) * 4 + 0] = 255; // R
				vector1d[(x + (y * texture_size.x)) * 4 + 1] = 255; // G
				vector1d[(x + (y * texture_size.x)) * 4 + 2] = 255; // B
				vector1d[(x + (y * texture_size.x)) * 4 + 3] = greyscale_value; // A
			}
		}

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		charset.generate_texture(glm::ivec2(vector2d[0].size(), vector2d.size()), &vector1d[0], vector1d.size());

		// reenable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		// Define sprites (we needed to know the size of the texture before we did this)
		float offset_x = 0.0f;
		for (unsigned char c = 0; c < 128; c++) {
			// Load character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph '" << c << "'" << std::endl;

				character ch;
				ch.character = c;
				ch.sprite_pos = glm::vec2(0.0f);
				ch.sprite_size = glm::vec2(0.0f);

				ch.glyph_size = glm::ivec2(0);
				ch.glyph_bearing = glm::ivec2(0);
				ch.advance = 0;

				chars.insert({ c, ch });

				continue;
			}


			character ch;
			ch.character = c;

			ch.sprite_pos = glm::vec2(offset_x, 1.0f);
			offset_x += static_cast<float>(face->glyph->bitmap.width) / static_cast<float>(texture_size.x);

			ch.sprite_size = glm::vec2(
				static_cast<float>(face->glyph->bitmap.width) / static_cast<float>(charset.get_size().x),
				static_cast<float>(face->glyph->bitmap.rows) / static_cast<float>(charset.get_size().y));

			ch.glyph_size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
			ch.glyph_bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
			ch.advance = face->glyph->advance.x;

			chars.insert({ c, ch });
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // reenable byte-alignment restriction
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Display_Charset::render() {
	display.render();
}

Render_Batch* Display_Charset::get_render_batch_ptr() {
	return &display;
}

glm::vec2 Display_Charset::get_sprite_pos(char c) {
	if (chars.find(c) == chars.end()) {
		return glm::vec2(0.0f); // searching for a char that is not in the charset should return the same as an unloaded char
	}
	else {
		return chars[c].sprite_pos;
	}
}

glm::vec2 Display_Charset::get_sprite_size(char c) {
	if (chars.find(c) == chars.end()) {
		return glm::vec2(0.0f); // searching for a char that is not in the charset should return the same as an unloaded char
	}
	else {
		return chars[c].sprite_size;
	}
}


glm::ivec2 Display_Charset::get_glyph_size(char c) {
	if (chars.find(c) == chars.end()) {
		return glm::ivec2(0); // searching for a char that is not in the charset should return the same as an unloaded char
	}
	else {
		return chars[c].glyph_size;
	}
}

glm::ivec2 Display_Charset::get_glyph_bearing(char c) {
	if (chars.find(c) == chars.end()) {
		return glm::ivec2(0); // searching for a char that is not in the charset should return the same as an unloaded char
	}
	else {
		return chars[c].glyph_bearing;
	}
}

unsigned int Display_Charset::get_advance(char c) {
	if (chars.find(c) == chars.end()) {
		return 0; // searching for a char that is not in the charset should return the same as an unloaded char
	}
	else {
		return chars[c].advance;
	}
}