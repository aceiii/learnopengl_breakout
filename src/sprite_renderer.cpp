#include "sprite_renderer.h"
#include "gl_check.h"

SpriteRenderer::SpriteRenderer(const Shader &shader)
    : _shader {shader}, _quad_vao {}
{
    initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    GL_CHECK(glDeleteVertexArrays(1, &_quad_vao));
}

void SpriteRenderer::drawSprite(Texture2D &texture, glm::vec2 position, glm::vec2 size, GLfloat rotate, glm::vec3 color) {
    _shader.use();

    glm::mat4 model;
    model = glm::translate(model, glm::vec3(position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    _shader.setMatrix4("model", model);
    _shader.setVector3f("sprite_color", color);

    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    texture.bind();

    GL_CHECK(glBindVertexArray(_quad_vao));
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
    GL_CHECK(glBindVertexArray(0));
}

void SpriteRenderer::initRenderData() {
    GLuint vbo;
    GLfloat vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
    };

    GL_CHECK(glGenVertexArrays(1, &_quad_vao));
    GL_CHECK(glGenBuffers(1, &vbo));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CHECK(glBindVertexArray(_quad_vao));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindVertexArray(0));
}

