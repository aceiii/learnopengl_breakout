#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"


class SpriteRenderer {
public:
    SpriteRenderer(const Shader &shader);
    ~SpriteRenderer();

    void drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
    void initRenderData();

    Shader _shader {};

    unsigned int _quad_vao;

};
