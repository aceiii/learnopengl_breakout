#pragma once

#include <glm/glm.hpp>
#include <string>

#include "game_object.h"


const glm::vec2 SIZE {60, 20};
const glm::vec2 VELOCITY {0.0f, 150.0f};

class PowerUp : public GameObject {
public:
    std::string type {};
    float duration {0};
    bool is_activated {false};

    PowerUp(const std::string &type, glm::vec3 color, float duration, glm::vec2 position, Texture2D texture);
};
