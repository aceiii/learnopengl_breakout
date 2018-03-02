#ifndef __GAME_HEADER_GUARD__
#define __GAME_HEADER_GUARD__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game_level.h"
#include "powerup.h"


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
    GameState state {GAME_MENU};
    std::vector<GameLevel> levels {};
    std::vector<PowerUp> powerups {};
    GLuint level {0};
    GLboolean keys[1024] {false};
    GLboolean keys_processed[1024] {false};
    GLuint width {0}, height {0};
    GLfloat scale {1.0f};
    GLuint lives {3};

    Game(GLuint width, GLuint height, GLfloat scale);
    ~Game();

    void init();
    void processInput(GLfloat dt);
    void update(GLfloat dt);
    void render();

    void doCollisions();

    void resetLevel();
    void resetPlayer();

    void spawnPowerUps(GameObject &block);
    void updatePowerUps(GLfloat dt);
};


#endif//__GAME_HEADER_GUARD__

