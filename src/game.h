#pragma once

#include "game_level.h"
#include "powerup.h"


const glm::vec2 PLAYER_SIZE {100, 20};
const float PLAYER_VELOCITY {500.0f};

const glm::vec2 INITIAL_BALL_VELOCITY {100.0f, -350.0f};
const float BALL_RADIUS {12.5f};

enum class GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

enum class Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT,
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

class Game {
public:
    GameState state {GameState::GAME_MENU};
    std::vector<GameLevel> levels {};
    std::vector<PowerUp> powerups {};
    unsigned int level {0};
    bool keys[1024] {false};
    bool keys_processed[1024] {false};
    unsigned int width {0}, height {0};
    float scale {1.0f};
    unsigned int lives {3};

    Game(unsigned int width, unsigned int height, float scale);
    ~Game();

    void init();
    void processInput(float dt);
    void update(float dt);
    void render();

    void doCollisions();

    void resetLevel();
    void resetPlayer();

    void spawnPowerUps(GameObject &block);
    void updatePowerUps(float dt);
};
