#include "game_object.h"

GameObject::GameObject()
    : position {0, 0}, size {1, 1},
      velocity {0.0f}, color {1.0f}, rotation {0.0f},
      sprite {}, is_solid {true}, is_destroyed {false}
{
}

GameObject::GameObject(glm::vec2 position_, glm::vec2 size_, Texture2D sprite_, glm::vec3 color_, glm::vec2 velocity_)
    : position {position_}, size {size_},
      velocity {velocity_}, color {color_}, rotation {0.0f},
      sprite {sprite_}, is_solid {false}, is_destroyed {false}
{
}

void GameObject::draw(SpriteRenderer &renderer) {
    renderer.drawSprite(sprite, position, size, rotation, color);
}

