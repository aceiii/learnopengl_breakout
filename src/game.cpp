#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

SpriteRenderer *renderer;
GameObject *player;

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
}

void Game::processInput(GLfloat dt) {
    if (state == GAME_ACTIVE) {
        GLfloat velocity = PLAYER_VELOCITY * dt;

        if (keys[GLFW_KEY_A]) {
            if (player->position.x >= 0) {
                player->position.x -= velocity;
            }
        }

        if (keys[GLFW_KEY_D]) {
            if (player->position.x <= width - player->size.x) {
                player->position.x += velocity;
            }
        }
    }
}

void Game::update(GLfloat dt) {
}

void Game::render() {
    if (state == GAME_ACTIVE) {
        renderer->drawSprite(ResourceManager::getTexture("background"),
                glm::vec2(0, 0), glm::vec2(width, height), 0.0f);

        levels[level].draw(*renderer);

        player->draw(*renderer);
    }
}

