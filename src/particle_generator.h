#ifndef __PARTICLE_GENERATOR_HEADER_GUARD__
#define __PARTICLE_GENERATOR_HEADER_GUARD__

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"


struct Particle {
    glm::vec2 position {0.0f};
    glm::vec2 velocity {0.0f};
    glm::vec4 color {1.0f};
    GLfloat life {0.0f};
};

class ParticleGenerator {
public:
    ParticleGenerator(Shader shader, Texture2D texture, GLuint amount);

    void update(GLfloat dt, GameObject &object, GLuint new_particles, glm::vec2 offset = {0.0f, 0.0f});
    void draw();

private:
    void init();
    GLuint firstUnusedParticle();
    void respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset = {0.0f, 0.0f});

    GLuint _vao;
    GLuint _amount;

    std::vector<Particle> _particles {};
    Shader _shader {};
    Texture2D _texture {};

};


#endif//__PARTICLE_GENERATOR_HEADER_GUARD__
