#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"


struct Particle {
    glm::vec2 position {0.0f};
    glm::vec2 velocity {0.0f};
    glm::vec4 color {1.0f};
    float life {0.0f};
};

class ParticleGenerator {
public:
    ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);

    void update(float dt, GameObject &object, unsigned int new_particles, glm::vec2 offset = {0.0f, 0.0f});
    void draw();

private:
    void init();
    unsigned int firstUnusedParticle();
    void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = {0.0f, 0.0f});

    unsigned int _vao;
    unsigned int _amount;

    std::vector<Particle> _particles {};
    Shader _shader {};
    Texture2D _texture {};

};
