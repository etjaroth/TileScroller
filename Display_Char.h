#pragma once
#include "Sprite.h"
#include "Display_Charset.h"

#include "ft2build.h"
;
#include FT_FREETYPE_H
;

class Display_Char : virtual public Sprite {
private:
    Display_Charset* display;
    char character;

public:
    Display_Char(Display_Charset* charset, char c, glm::vec2 pos, glm::vec2 size);
    
    void set_char(char c);
    char get_char();

    glm::ivec2 get_glyph_size(char c);
    glm::ivec2 get_glyph_bearing(char c);
    unsigned int get_advance(char c);
};

