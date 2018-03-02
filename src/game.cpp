#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "post_process.h"
#include "powerup.h"
#include "mp3audio.h"
#include "text_renderer.h"

#include <soloud/soloud.h>
#include <soloud/soloud_wav.h>

#include <sstream>


SpriteRenderer *renderer;
GameObject *player;
BallObject *ball;
ParticleGenerator *particles;
PostProcess *effects;
TextRenderer *text;
GLfloat shake_time = 0.0f;
SoLoud::Soloud soloud;
MP3Audio music_bg;
MP3Audio sfx_bleep1;
SoLoud::Wav sfx_bleep2;
SoLoud::Wav sfx_powerup;
SoLoud::Wav sfx_solid;

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

GLboolean checkCollision(GameObject &a, GameObject &b) {
    bool collision_x = a.position.x + a.size.x >= b.position.x && b.position.x + b.size.x >= a.position.x;
    bool collision_y = a.position.y + a.size.y >= b.position.y && b.position.y + b.size.y >= a.position.y;

    return collision_x && collision_y;
}

Collision checkCollision(BallObject &a, GameObject &b) {
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

GLboolean shouldSpawn(GLuint chance) {
    GLuint random = rand() % chance;
    return random == 0;
}

void activatePowerUp(PowerUp &powerup) {
    if (powerup.type == "speed") {
        ball->velocity *= 1.2f;
    }
    else if (powerup.type == "sticky") {
        ball->is_sticky = GL_TRUE;
        player->color = glm::vec3 {1.0f, 0.5f, 1.0f};
    }
    else if (powerup.type == "pass-through") {
        ball->is_passthrough = GL_TRUE;
        ball->color = glm::vec3 {1.0f, 0.5f, 0.5f};
    }
    else if (powerup.type == "pad-size-increase") {
        player->size.x += 50;
    }
    else if (powerup.type == "confuse") {
        if (!effects->chaos) {
            effects->confuse = GL_TRUE;
        }
    }
    else if (powerup.type == "chaos") {
        if (!effects->confuse) {
            effects->chaos = GL_TRUE;
        }
    }
}

GLboolean isOtherPowerUpActive(std::vector<PowerUp> &powerups, const std::string &type) {
    for (const PowerUp &powerup : powerups) {
        if (powerup.is_activated) {
            if (powerup.type == type) {
                return GL_TRUE;
            }
        }
    }

    return GL_FALSE;
}

Game::Game(GLuint width_, GLuint height_, GLfloat scale_)
    : state {GAME_MENU}, keys {}, width {width_}, height {height_}, scale {scale_}, lives {3}
{
}

Game::~Game() {
}

void Game::init() {
    soloud.init();

    ResourceManager::loadShader("res/shaders/sprite_vs.glsl", "res/shaders/sprite_fs.glsl", nullptr, "sprite");
    ResourceManager::loadShader("res/shaders/particle_vs.glsl", "res/shaders/particle_fs.glsl", nullptr, "particle");
    ResourceManager::loadShader("res/shaders/postprocessing_vs.glsl", "res/shaders/postprocessing_fs.glsl", nullptr, "postprocessing");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f, -1.0f, 1.0f);

    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);
    ResourceManager::getShader("particle").use().setInteger("image", 0);
    ResourceManager::getShader("particle").setMatrix4("projection", projection);

    ResourceManager::loadTexture("res/textures/awesomeface.png", GL_TRUE, "face");
    ResourceManager::loadTexture("res/textures/background.jpg", GL_FALSE, "background");
    ResourceManager::loadTexture("res/textures/block.png", GL_FALSE, "block");
    ResourceManager::loadTexture("res/textures/block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::loadTexture("res/textures/paddle.png", GL_TRUE, "paddle");
    ResourceManager::loadTexture("res/textures/particle.png", GL_TRUE, "particle");
    ResourceManager::loadTexture("res/textures/powerup_speed.png", GL_TRUE, "powerup_speed");
    ResourceManager::loadTexture("res/textures/powerup_sticky.png", GL_TRUE, "powerup_sticky");
    ResourceManager::loadTexture("res/textures/powerup_increase.png", GL_TRUE, "powerup_increase");
    ResourceManager::loadTexture("res/textures/powerup_confuse.png", GL_TRUE, "powerup_confuse");
    ResourceManager::loadTexture("res/textures/powerup_chaos.png", GL_TRUE, "powerup_chaos");
    ResourceManager::loadTexture("res/textures/powerup_passthrough.png", GL_TRUE, "powerup_passthrough");

    renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));
    particles = new ParticleGenerator(ResourceManager::getShader("particle"), ResourceManager::getTexture("particle"), 500);
    effects = new PostProcess(ResourceManager::getShader("postprocessing"), width * scale, height * scale);

    text = new TextRenderer(width, height);
    text->load("res/fonts/ocraext.ttf", 24);

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
    glm::vec2 ball_pos {player_pos + glm::vec2 {PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2}};

    player = new GameObject {player_pos, PLAYER_SIZE, ResourceManager::getTexture("paddle")};
    ball = new BallObject {ball_pos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face")};

    music_bg.load("res/audio/breakout.mp3");
    music_bg.setLooping(true);

    sfx_bleep1.load("res/audio/bleep.mp3");
    sfx_bleep2.load("res/audio/bleep.wav");
    sfx_powerup.load("res/audio/powerup.wav");
    sfx_solid.load("res/audio/solid.wav");

    soloud.play(music_bg);
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

        if (keys[GLFW_KEY_1]) {
            effects->confuse = !effects->confuse;
        }

        if (keys[GLFW_KEY_2]) {
            effects->chaos = !effects->chaos;
        }
    }

    if (state == GAME_MENU) {
        if (keys[GLFW_KEY_ENTER] && !keys_processed[GLFW_KEY_ENTER]) {
            keys_processed[GLFW_KEY_ENTER] = GL_TRUE;
            state = GAME_ACTIVE;
        }

        if (keys[GLFW_KEY_W] && !keys_processed[GLFW_KEY_W]) {
            keys_processed[GLFW_KEY_W] = GL_TRUE;
            level = (level + 1) % levels.size();
        }

        if (keys[GLFW_KEY_S] && !keys_processed[GLFW_KEY_S]) {
            keys_processed[GLFW_KEY_S] = GL_TRUE;
            if (level > 0) {
                level -= 1;
            } else {
                level = levels.size() - 1;
            }
        }

    }
}

void Game::update(GLfloat dt) {
    ball->move(dt, width);

    doCollisions();

    particles->update(dt, *ball, 2, glm::vec2(ball->radius / 2));

    updatePowerUps(dt);

    if (shake_time > 0.0f) {
        shake_time -= dt;
        if (shake_time <= 0.0f) {
            effects->shake = GL_FALSE;
        }
    }

    if (ball->position.y >= height) {
        lives -= 1;

        if (lives == 0) {
            resetLevel();
            state = GAME_MENU;
        }

        resetPlayer();
    }
}

void Game::render() {
    if (state == GAME_ACTIVE || state == GAME_MENU) {

        effects->beginRender();
        {
            renderer->drawSprite(ResourceManager::getTexture("background"),
                    glm::vec2(0, 0), glm::vec2(width, height), 0.0f);

            levels[level].draw(*renderer);

            for (PowerUp &powerup : powerups) {
                if (!powerup.is_destroyed) {
                    powerup.draw(*renderer);
                }
            }

            player->draw(*renderer);
            particles->draw();
            ball->draw(*renderer);
        }
        effects->endRender();
        effects->render(glfwGetTime());

        std::stringstream ss;
        ss << lives;

        text->renderText("Lives: " + ss.str(), 5.0f, 5.0f, 1.0f);
    }

    if (state == GAME_MENU) {
        text->renderText("Press ENTER to start", 250.0f, height / 2.0, 1.0f);
        text->renderText("Press W or S to select a level", 245.0f, height / 2.0 + 20.0f, 0.75f);
    }
}

void Game::doCollisions() {
    for (auto &box : levels[level].bricks) {
        if (!box.is_destroyed) {
            Collision collision = checkCollision(*ball, box);

            if (std::get<0>(collision)) {
                if (!box.is_solid) {
                    box.is_destroyed = GL_TRUE;
                    spawnPowerUps(box);
                    soloud.play(sfx_bleep1);
                } else {
                    shake_time = 0.05f;
                    effects->shake = GL_TRUE;
                    soloud.play(sfx_solid);
                }

                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);

                if (!(ball->is_passthrough && !box.is_solid)) {
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
    }

    Collision collision = checkCollision(*ball, *player);
    if (!ball->is_stuck && std::get<0>(collision)) {
        GLfloat center_board = player->position.x + (player->size.x / 2);
        GLfloat distance = (ball->position.x + ball->radius) - center_board;
        GLfloat percentage = distance / (player->size.x / 2);

        const GLfloat strength = 2.0f;
        const glm::vec2 old_velocity = ball->velocity;

        ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->velocity.y = -1 * abs(ball->velocity.y);
        ball->velocity = glm::normalize(ball->velocity) * glm::length(old_velocity);
        ball->is_stuck = ball->is_sticky;

        soloud.play(sfx_bleep2);
    }

    for (PowerUp &powerup : powerups) {
        if (!powerup.is_destroyed) {
            if (powerup.position.y >= height) {
                powerup.is_destroyed = GL_TRUE;
            }

            if (checkCollision(*player, powerup)) {
                activatePowerUp(powerup);
                powerup.is_destroyed = GL_TRUE;
                powerup.is_activated = GL_TRUE;

                soloud.play(sfx_powerup);
            }
        }
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

    lives = 3;
}

void Game::resetPlayer() {
    player->size = PLAYER_SIZE;
    player->position = glm::vec2(width / 2 - PLAYER_SIZE.x / 2, height - PLAYER_SIZE.y);

    ball->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
}

void Game::spawnPowerUps(GameObject &block) {
    if (shouldSpawn(15)) {
        powerups.push_back({"speed", {0.5f, 0.5f, 1.0f}, 0.0f, block.position, ResourceManager::getTexture("powerup_speed")});
    }
    if (shouldSpawn(15)) {
        powerups.push_back({"sticky", {1.0f, 0.5f, 1.0f}, 20.0f, block.position, ResourceManager::getTexture("powerup_sticky")});
    }
    if (shouldSpawn(15)) {
        powerups.push_back({"pass-through", {0.5f, 1.0f, 0.5f}, 10.0f, block.position, ResourceManager::getTexture("powerup_passthrough")});
    }
    if (shouldSpawn(15)) {
        powerups.push_back({"pad-size-increase", {1.0f, 0.6f, 0.4f}, 0.0f, block.position, ResourceManager::getTexture("powerup_increase")});
    }
    if (shouldSpawn(15)) {
        powerups.push_back({"confuse", {1.0f, 0.3f, 0.3f}, 15.0f, block.position, ResourceManager::getTexture("powerup_confuse")});
    }
    if (shouldSpawn(15)) {
        powerups.push_back({"chaos", {0.9f, 0.25f, 0.25f}, 15.0f, block.position, ResourceManager::getTexture("powerup_chaos")});
    }
}

void Game::updatePowerUps(GLfloat dt) {
    for (PowerUp &powerup : powerups) {
        powerup.position += powerup.velocity * dt;
        if (powerup.is_activated) {
            powerup.duration -= dt;

            if (powerup.duration <= 0.0f) {

                powerup.is_activated = GL_FALSE;

                if (powerup.type == "sticky") {
                    if (!isOtherPowerUpActive(powerups, "sticky")) {
                        ball->is_sticky = GL_FALSE;
                        player->color = glm::vec3 {1.0f};
                    }
                }
                else if (powerup.type == "pass-through") {
                    if (!isOtherPowerUpActive(powerups, "pass-through")) {
                        ball->is_passthrough = GL_FALSE;
                        ball->color = glm::vec3 {1.0f};
                    }
                }
                else if (powerup.type == "confuse") {
                    if (!isOtherPowerUpActive(powerups, "confuse")) {
                        effects->confuse = GL_FALSE;
                    }
                }
                else if (powerup.type == "chaos") {
                    if (!isOtherPowerUpActive(powerups, "chaos")) {
                        effects->chaos = GL_FALSE;
                    }
                }
            }
        }
    }

    powerups.erase(std::remove_if(powerups.begin(), powerups.end(), [] (const PowerUp &powerup) {
        return powerup.is_destroyed && !powerup.is_activated;
    }), powerups.end());
}

