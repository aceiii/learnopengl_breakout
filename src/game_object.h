#ifndef __GAME_OBJECT_HEADER_GUARD__
#define __GAME_OBJECT_HEADER_GUARD__

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"


class GameObject {
public:

    glm::vec2 position;
    glm::vec2 size;
    glm::vec2 velocity;
    glm::vec3 color;

    GLfloat rotation;
    GLboolean is_solid;
    GLboolean is_destroyed;

    Texture2D sprite;

    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

    virtual void draw(SpriteRenderer &renderer);

};


#endif//__GAME_OBJECT_HEADER_GUARD__
