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

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
};

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

class Game {
public:
    GameState state {GAME_ACTIVE};
    std::vector<GameLevel> levels {};
    GLuint level {0};
    GLboolean keys[1024] {0};
    GLuint width {0}, height {0};
    GLfloat scale {1.0f};

    Game(GLuint width, GLuint height, GLfloat scale);
    ~Game();

    void init();
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();

    void doCollisions();

    void resetLevel();
    void resetPlayer();
};


#endif//__GAME_HEADER_GUARD__

