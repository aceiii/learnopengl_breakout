#ifndef __TEXT_RENDERER_HEADER_GUARD__
#define __TEXT_RENDERER_HEADER_GUARD__

#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "shader.h"


struct Character {
    GLuint texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};

class TextRenderer {
public:
    std::map<GLchar, Character> characters;
    Shader text_shader;

    TextRenderer(GLuint width, GLuint height);

    void load(const std::string &font, GLuint font_size);
    void renderText(const std::string &text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color = glm::vec3 {1.0f});

private:
    GLuint _vao;
    GLuint _vbo;

};


#endif//__TEXT_RENDERER_HEADER_GUARD__
