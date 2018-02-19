#include "particle_generator.h"
#include "gl_check.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, GLuint amount)
    : _amount {amount}, _shader {shader}, _texture {texture}
{
    init();
}

void ParticleGenerator::update(GLfloat dt, GameObject &object, GLuint new_particles, glm::vec2 offset) {
    for (GLuint i = 0; i < new_particles; i += 1) {
        int unused_particle = firstUnusedParticle();
        respawnParticle(_particles[unused_particle], object, offset);
    }

    for (GLuint i = 0; i < _amount; i += 1) {
        Particle &p = _particles[i];

        p.life -= dt;
        if (p.life > 0.0f) {
            p.position -= p.velocity * dt;
            p.color.a -= dt * 2.5f;
        }
    }
}

void ParticleGenerator::draw() {
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE));

    _shader.use();

    for (const Particle &particle : _particles) {
        if (particle.life > 0.0f) {
            _shader.setVector2f("offset", particle.position);
            _shader.setVector4f("color", particle.color);
            _texture.bind();

            GL_CHECK(glBindVertexArray(_vao));
            GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
            GL_CHECK(glBindVertexArray(0));
        }
    }

    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
}

void ParticleGenerator::init() {
    GLuint vbo;
    GLfloat particle_quad[] {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
    };

    GL_CHECK(glGenVertexArrays(1, &_vao));
    GL_CHECK(glGenBuffers(1, &vbo));

    GL_CHECK(glBindVertexArray(_vao));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0));
    GL_CHECK(glBindVertexArray(0));

    for (GLuint i = 0; i < _amount; i += 1) {
        _particles.push_back({});
    }
}

GLuint ParticleGenerator::firstUnusedParticle() {
    static GLuint last_used_particle {0};

    for (GLuint i = last_used_particle; i < _amount; i += 1) {
        if (_particles[i].life <= 0.0f) {
            last_used_particle = i;
            return i;
        }
    }

    for (GLuint i = 0; i < last_used_particle; i += 1) {
        if (_particles[i].life <= 0.0f) {
            last_used_particle = i;
            return i;
        }
    }

    last_used_particle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, GameObject &object, glm::vec2 offset) {
    GLfloat random = ((rand() % 100) - 50) / 10.0f;
    GLfloat r_color = 0.5 + ((rand() % 100) / 100.0f);

    particle.position = object.position + random + offset;
    particle.color = glm::vec4(r_color, r_color, r_color, 1.0f);
    particle.life = 1.0f;
    particle.velocity = object.velocity * 0.1f;
}

