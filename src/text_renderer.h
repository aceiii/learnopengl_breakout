#pragma once

#include <map>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"


struct Character {
    unsigned int texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    unsigned int advance;
};

class TextRenderer {
public:
    std::map<char, Character> characters;
    Shader text_shader;

    TextRenderer(unsigned int width, unsigned int height);

    void load(std::string_view font, unsigned int font_size);
    void renderText(std::string_view text, float x, float y, float scale, glm::vec3 color = glm::vec3 {1.0f});

private:
    unsigned int _vao {};
    unsigned int _vbo {};

};
