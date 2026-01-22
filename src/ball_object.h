#ifndef __BALL_OBJECT_HEADER_GUARD__
#define __BALL_OBJECT_HEADER_GUARD__

#include <glm/glm.hpp>

#include "game_object.h"


class BallObject : public GameObject {
public:
    float radius {1.0f};
    bool is_stuck {false};
    bool is_sticky {false};
    bool is_passthrough {false};

    BallObject();
    BallObject(glm::vec2 position, float radius, glm::vec2 velocity, Texture2D sprite);

    glm::vec2 move(float dt, unsigned int window_width);
    void reset(glm::vec2 position, glm::vec2 velocity);
};


#endif//__BALL_OBJECT_HEADER_GUARD__
