#include "ball_object.h"

BallObject::BallObject()
    : GameObject {}, radius {12.5f}, is_stuck {true}
{
}

BallObject::BallObject(glm::vec2 position_, GLfloat radius_, glm::vec2 velocity_, Texture2D sprite)
    : GameObject {position_, glm::vec2 {radius_ * 2, radius_ * 2}, sprite, glm::vec3 {1.0f}, velocity_},
      radius {radius_}, is_stuck {true}
{
}

glm::vec2 BallObject::move(GLfloat dt, GLuint window_width) {
    if (!is_stuck) {
        position += velocity * dt;

        if (position.x <= 0.0f) {
            velocity.x = -velocity.x;
            position.x = 0.0f;
        }
        else if (position.x + size.x >= window_width) {
            velocity.x = -velocity.x;
            position.x = window_width - size.x;
        }
        else if (position.y <= 0.0f) {
            velocity.y = -velocity.y;
            position.y = 0.0f;
        }
    }

    return position;
}

void BallObject::reset(glm::vec2 position_, glm::vec2 velocity_) {
    position = position_;
    velocity = velocity_;
    is_stuck = true;
}

