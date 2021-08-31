#pragma once
#include "Sprite.h"
#include "Display_Charset.h"

#include "ft2build.h"
#include FT_FREETYPE_H

class Display_Char :
    public Sprite
{
public:
    Display_Char(Render_Batch* sheet, int sprite, glm::vec2 pos, glm::vec2 size);
};

