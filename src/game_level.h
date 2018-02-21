#ifndef __GAME_LEVEL_HEADER_GUARD__
#define __GAME_LEVEL_HEADER_GUARD__

#include <GL/glew.h>
#include <vector>

#include "game_object.h"


class GameLevel {
public:
    std::vector<GameObject> bricks {0};

    GameLevel();

    void load(const GLchar *filename, GLuint level_width, GLuint level_height);

    void draw(SpriteRenderer &renderer);

    GLboolean isCompleted();

private:
    void init(std::vector<std::vector<GLuint>> tile_data, GLuint level_width, GLuint level_height);
};


#endif//__GAME_LEVEL_HEADER_GUARD__
