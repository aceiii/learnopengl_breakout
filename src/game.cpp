#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

SpriteRenderer *renderer;

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
}

void Game::processInput(GLfloat dt) {
}

void Game::update(GLfloat dt) {
}

void Game::render() {
    renderer->drawSprite(ResourceManager::getTexture("face"), glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

