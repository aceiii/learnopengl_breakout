#include "post_process.h"
#include "gl_check.h"

#include <iostream>

PostProcess::PostProcess(Shader shader_, GLuint width_, GLuint height_)
    : post_processing_shader {shader_}, width {width_}, height {height_}
{
    //std::cout << "PostProcess ( " << width << ',' << height << " )\n";

    GL_CHECK(glGenFramebuffers(1, &_msfbo));
    GL_CHECK(glGenFramebuffers(1, &_fbo));
    GL_CHECK(glGenRenderbuffers(1, &_rbo));

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, _msfbo));
    GL_CHECK(glBindRenderbuffer(GL_RENDERBUFFER, _rbo));
    GL_CHECK(glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height));
    GL_CHECK(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _rbo));

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO\n";
    }

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, _fbo));

    texture.generate(width, height, nullptr);

    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.id, 0));
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO\n";
    }

    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    initRenderData();

    post_processing_shader.setInteger("scene", 0, GL_TRUE);

    GLfloat offset = 1.0f / 300.0f;
    GLfloat offsets[9][2] = {
        {-offset, offset},
        { 0.0f, offset},
        {offset, offset},
        {-offset, 0.0f},
        {0.0f, 0.0f},
        {offset, 0.0f},
        {-offset, -offset},
        {0.0f, -offset},
        {offset, -offset},
    };

    GLint edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1,
    };

    GLfloat blur_kernel[9] = {
        1.0f / 16, 2.0f / 16, 1.0f / 16,
        2.0f / 16, 4.0f / 16, 2.0f / 16,
        1.0f / 16, 2.0f / 16, 1.0f / 16,
    };

    GL_CHECK(glUniform2fv(glGetUniformLocation(post_processing_shader.id, "offsets"), 9, (GLfloat *)offsets));
    GL_CHECK(glUniform1iv(glGetUniformLocation(post_processing_shader.id, "edge_kernel"), 9, edge_kernel));
    GL_CHECK(glUniform1fv(glGetUniformLocation(post_processing_shader.id, "blur_kernel"), 9, blur_kernel));
}

void PostProcess::beginRender() {
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, _msfbo));
    GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
}

void PostProcess::endRender() {
    GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, _msfbo));
    GL_CHECK(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo));
    GL_CHECK(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR));
    GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void PostProcess::render(GLfloat time) {
    post_processing_shader.use();
    post_processing_shader.setFloat("time", time);
    post_processing_shader.setInteger("confuse", confuse);
    post_processing_shader.setInteger("chaos", chaos);
    post_processing_shader.setInteger("shake", shake);

    GL_CHECK(glActiveTexture(GL_TEXTURE0));

    texture.bind();

    GL_CHECK(glBindVertexArray(_vao));
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
    GL_CHECK(glBindVertexArray(0));
}

void PostProcess::initRenderData() {
    GLuint vbo;
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
    };

    GL_CHECK(glGenVertexArrays(1, &_vao));
    GL_CHECK(glGenBuffers(1, &vbo));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CHECK(glBindVertexArray(_vao));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid *)0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindVertexArray(0));
}

