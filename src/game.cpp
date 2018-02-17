#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"

SpriteRenderer *renderer;
GameObject *player;
BallObject *ball;

GLboolean checkCollisions(GameObject &a, GameObject &b) {
    bool collision_x = a.position.x + a.size.x >= b.position.x && b.position.x + b.size.x >= a.position.x;
    bool collision_y = a.position.y + a.size.y >= b.position.y && b.position.y + b.size.y >= a.position.y;

    return collision_x && collision_y;
}

Game::Game(GLuint width_, GLuint height_)
    : state {GAME_ACTIVE}, keys {}, width {width_}, height {height_}
{
}

Game::~Game() {
}

void Game::init() {
    ResourceManager::loadShader("res/shaders/sprite_vs.glsl", "res/shaders/sprite_fs.glsl", nullptr, "sprite");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);

    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").use().setMatrix4("projection", projection);

    renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));

    ResourceManager::loadTexture("res/textures/awesomeface.png", GL_TRUE, "face");

    ResourceManager::loadTexture("res/textures/background.jpg", GL_FALSE, "background");
    ResourceManager::loadTexture("res/textures/block.png", GL_FALSE, "block");
    ResourceManager::loadTexture("res/textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::loadTexture("res/textures/paddle.png", GL_TRUE, "paddle");

    GameLevel one, two, three, four;
    one.load("res/levels/one.level", width, height * 0.5f);
    two.load("res/levels/two.level", width, height * 0.5f);
    three.load("res/levels/three.level", width, height * 0.5f);
    four.load("res/levels/four.level", width, height * 0.5f);

    levels.push_back(one);
    levels.push_back(two);
    levels.push_back(three);
    levels.push_back(four);

    glm::vec2 player_pos {width / 2 - PLAYER_SIZE.x / 2, height - PLAYER_SIZE.y};

    player = new GameObject {player_pos, PLAYER_SIZE, ResourceManager::getTexture("paddle")};

    glm::vec2 ball_pos {player_pos + glm::vec2 {PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2}};
    ball = new BallObject {ball_pos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face")};
}

void Game::processInput(GLfloat dt) {
    if (state == GAME_ACTIVE) {
        GLfloat velocity = PLAYER_VELOCITY * dt;

        if (keys[GLFW_KEY_A]) {
            if (player->position.x >= 0) {
                player->position.x -= velocity;

                if (ball->is_stuck) {
                    ball->position.x -= velocity;
                }
            }
        }

        if (keys[GLFW_KEY_D]) {
            if (player->position.x <= width - player->size.x) {
                player->position.x += velocity;

                if (ball->is_stuck) {
                    ball->position.x += velocity;
                }
            }
        }

        if (keys[GLFW_KEY_SPACE]) {
            ball->is_stuck = false;
        }
    }
}

void Game::update(GLfloat dt) {
    ball->move(dt, width);
    doCollisions();
}

void Game::render() {
    if (state == GAME_ACTIVE) {
        renderer->drawSprite(ResourceManager::getTexture("background"),
                glm::vec2(0, 0), glm::vec2(width, height), 0.0f);

        levels[level].draw(*renderer);

        player->draw(*renderer);
        ball->draw(*renderer);
    }
}

void Game::doCollisions() {
    for (auto &box : levels[level].bricks) {
        if (!box.is_destroyed) {
            if (checkCollisions(*ball, box)) {
                if (!box.is_solid) {
                    box.is_destroyed = GL_TRUE;
                }
            }
        }
    }
}

