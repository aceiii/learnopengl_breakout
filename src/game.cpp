#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"

SpriteRenderer *renderer;
GameObject *player;
BallObject *ball;

Direction vectorDirection(glm::vec2 target) {
    glm::vec2 compass[] = {
        {0.0f, 1.0f},  // up
        {1.0f, 0.0f},  // right
        {0.0f, -1.0f}, // down
        {-1.0f, 0.0f}, //left
    };

    GLfloat max {0.0f};
    GLuint best_match = -1;

    for (GLuint i = 0; i < 4; i += 1) {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            max = dot_product;
            best_match = i;
        }
    }

    return (Direction)best_match;
}

GLboolean checkCollisions(GameObject &a, GameObject &b) {
    bool collision_x = a.position.x + a.size.x >= b.position.x && b.position.x + b.size.x >= a.position.x;
    bool collision_y = a.position.y + a.size.y >= b.position.y && b.position.y + b.size.y >= a.position.y;

    return collision_x && collision_y;
}

Collision checkCollisions(BallObject &a, GameObject &b) {
    glm::vec2 center {a.position + a.radius};
    glm::vec2 aabb_half_extents {b.size.x / 2, b.size.y / 2};
    glm::vec2 aabb_center {
        b.position.x + aabb_half_extents.x,
        b.position.y + aabb_half_extents.y};
    glm::vec2 difference {center - aabb_center};
    glm::vec2 clamped {glm::clamp(difference, -aabb_half_extents, aabb_half_extents)};
    glm::vec2 closest {aabb_center + clamped};

    difference = closest - center;

    if (glm::length(difference) <= a.radius) {
        return std::make_tuple(GL_TRUE, vectorDirection(difference), difference);
    }

    return std::make_tuple(GL_FALSE, UP, glm::vec2 {0, 0});
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

    if (ball->position.y >= height) {
        resetLevel();
        resetPlayer();
    }
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
            Collision collision = checkCollisions(*ball, box);

            if (std::get<0>(collision)) {
                if (!box.is_solid) {
                    box.is_destroyed = GL_TRUE;
                }

                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);

                if (dir == LEFT || dir == RIGHT) {
                    ball->velocity.x = -ball->velocity.x;

                    GLfloat penetration = ball->radius - std::abs(diff_vector.x);
                    if (dir == LEFT) {
                        ball->position.x += penetration;
                    } else {
                        ball->position.x -= penetration;
                    }
                } else {
                    ball->velocity.y = -ball->velocity.y;

                    GLfloat penetration = ball->radius - std::abs(diff_vector.y);
                    if (dir == UP) {
                        ball->position.y -= penetration;
                    } else {
                        ball->position.y += penetration;
                    }
                }
            }
        }
    }

    Collision collision = checkCollisions(*ball, *player);
    if (!ball->is_stuck && std::get<0>(collision)) {
        GLfloat center_board = player->position.x + (player->size.x / 2);
        GLfloat distance = (ball->position.x + ball->radius) - center_board;
        GLfloat percentage = distance / (player->size.x / 2);

        const GLfloat strength = 2.0f;
        const glm::vec2 old_velocity = ball->velocity;

        ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->velocity.y = -1 * abs(ball->velocity.y);
        ball->velocity = glm::normalize(ball->velocity) * glm::length(old_velocity);
    }
}

void Game::resetLevel() {
    if (level == 0) {
        levels[0].load("res/levels/one.level", width, height * 0.5f);
    } else if (level == 1) {
        levels[1].load("res/levels/two.level", width, height * 0.5f);
    } else if (level == 2) {
        levels[2].load("res/levels/three.level", width, height * 0.5f);
    } else if (level == 3) {
        levels[3].load("res/levels/four.level", width, height * 0.5f);
    }
}

void Game::resetPlayer() {
    player->size = PLAYER_SIZE;
    player->position = glm::vec2(width / 2 - PLAYER_SIZE.x / 2, height - PLAYER_SIZE.y);

    ball->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}

