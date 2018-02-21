#ifndef __POWER_UP_HEADER_GUARD__
#define __POWER_UP_HEADER_GUARD__

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>

#include "game_object.h"


const glm::vec2 SIZE {60, 20};
const glm::vec2 VELOCITY {0.0f, 150.0f};

class PowerUp : public GameObject {
public:
    std::string type {};
    GLfloat duration {0};
    GLboolean is_activated {false};

    PowerUp(const std::string &type, glm::vec3 color, GLfloat duration, glm::vec2 position, Texture2D texture);
};


#endif//__POWER_UP_HEADER_GUARD__
