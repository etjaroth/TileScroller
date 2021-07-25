#pragma once

#include "opengl_libs.h"
#include "Collision_Box.h"
#include "Sprite.h"
class Entity :
    public Collision_Box, public Sprite
{
public:
    Entity(Render_Batch* sheet, int sprite, glm::vec2 pos, glm::vec2 size);
    Entity(Render_Batch* sheet, glm::ivec2 sprite, glm::vec2 pos, glm::vec2 size);

    void set_pos(glm::vec2 new_pos) override;
};