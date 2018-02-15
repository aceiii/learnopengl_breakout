#include "game.h"

Game::Game(GLuint width_, GLuint height_)
    : state {GAME_ACTIVE}, keys {}, width {width_}, height {height_}
{
}

Game::~Game() {
}

void Game::init() {
}

void Game::processInput(GLfloat dt) {
}

void Game::update(GLfloat dt) {
}

void Game::render() {
}

