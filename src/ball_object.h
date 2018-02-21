#ifndef __BALL_OBJECT_HEADER_GUARD__
#define __BALL_OBJECT_HEADER_GUARD__

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "game_object.h"


class BallObject : public GameObject {
public:
    GLfloat radius {1.0f};
    GLboolean is_stuck {false};

    BallObject();
    BallObject(glm::vec2 position, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

    glm::vec2 move(GLfloat dt, GLuint window_width);
    void reset(glm::vec2 position, glm::vec2 velocity);
};


#endif//__BALL_OBJECT_HEADER_GUARD__
