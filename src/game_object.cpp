#include "game_object.h"

GameObject::GameObject()
    : position {0, 0}, size {1, 1}, color {1.0f},
      is_solid {true}
{
}

GameObject::GameObject(glm::vec2 position_, glm::vec2 size_, Texture2D sprite_, glm::vec3 color_, glm::vec2 velocity_)
    : position {position_}, size {size_},
      velocity {velocity_}, color {color_}, sprite {sprite_}
{
}

GameObject::~GameObject()  = default;

void GameObject::draw(SpriteRenderer &renderer) {
    renderer.drawSprite(sprite, position, size, rotation, color);
}

