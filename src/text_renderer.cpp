#include "text_renderer.h"
#include "resource_manager.h"
#include "gl_check.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(GLuint width, GLuint height) {
    text_shader = ResourceManager::loadShader("shaders/text_vs.glsl", "shaders/text_fs.glsl", nullptr, "text");
    text_shader.setMatrix4("projection",  glm::ortho(0.0f, static_cast<GLfloat>(width), static_cast<GLfloat>(height), 0.0f), GL_TRUE);
    text_shader.setInteger("text", 0);

    GL_CHECK(glGenVertexArrays(1, &_vao));
    GL_CHECK(glGenBuffers(1, &_vbo));
    GL_CHECK(glBindVertexArray(_vao));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, nullptr, GL_DYNAMIC_DRAW));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindVertexArray(0));
}

void TextRenderer::load(const std::string &font, GLuint font_size) {
    characters.clear();

    FT_Library ft;
    if (!FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
    }

    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font '" << font << "'\n";
    }

    FT_Set_Pixel_Sizes(face, 0, font_size);

    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    for (GLubyte c = 0; c < 128; c += 1) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cout << "ERROR::FREETYPE: Failed to load Glyph\n";
            continue;
        }

        GLuint texture;
        GL_CHECK(glGenTextures(1, &texture));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture));
        GL_CHECK(glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width, face->glyph->bitmap.rows,
            0, GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        Character character = {
            texture,
            glm::ivec2 {face->glyph->bitmap.width, face->glyph->bitmap.rows},
            glm::ivec2 {face->glyph->bitmap_left, face->glyph->bitmap_top},
            GLuint(face->glyph->advance.x)
        };

        characters.insert(std::pair<GLchar, Character>(c, character));
    }

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::renderText(const std::string &text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    text_shader.use();
    text_shader.setVector3f("text_color", color);

    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindVertexArray(_vao));

    for (auto c = text.cbegin(); c != text.end(); c++) {
        Character ch = characters[*c];

        GLfloat xpos = x + ch.bearing.x * scale;
        GLfloat ypos = y + (characters['H'].bearing.y - ch.bearing.y) * scale;

        GLfloat w = ch.size.x * scale;
        GLfloat h = ch.size.y * scale;

        GLfloat vertices[6][4] = {
            {xpos,      ypos + h,   0.0f, 1.0f},
            {xpos + w,  ypos,       1.0f, 0.0f},
            {xpos,      ypos,       0.0f, 0.0f},
            {xpos,      ypos + h,   0.0f, 1.0f},
            {xpos + w,  ypos + h,   1.0f, 1.0f},
            {xpos + w,  ypos,       1.0f, 0.0f},
        };

        GL_CHECK(glBindTexture(GL_TEXTURE_2D, ch.texture_id));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vbo));
        GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));

        x += (ch.advance >> 6) * scale;
    }

    GL_CHECK(glBindVertexArray(0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}
