#ifndef __GAME_HEADER_GUARD__
#define __GAME_HEADER_GUARD__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game_level.h"


const glm::vec2 PLAYER_SIZE {100, 20};
const GLfloat PLAYER_VELOCITY {500.0f};

const glm::vec2 INITIAL_BALL_VELOCITY {100.0f, -350.0f};
const GLfloat BALL_RADIUS {12.5f};

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

class Game {
public:
    GameState state;
    std::vector<GameLevel> levels;
    GLuint level;
    GLboolean keys[1024];
    GLuint width, height;

    Game(GLuint width, GLuint height);
    ~Game();

    void init();
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();

    void doCollisions();
};


#endif//__GAME_HEADER_GUARD__

