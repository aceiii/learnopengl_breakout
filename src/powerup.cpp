#include "powerup.h"

PowerUp::PowerUp(const std::string &type_, glm::vec3 color_, float duration_, glm::vec2 pos_, Texture2D texture_)
    : GameObject {pos_, SIZE, texture_, color_, VELOCITY},
      type {type_}, duration {duration_}, is_activated {false}
{
}

