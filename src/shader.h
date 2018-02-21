#ifndef __SHADER_HEADER_GUARD__
#define __SHADER_HEADER_GUARD__

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader {
public:
    GLuint id {0};

    Shader();
    ~Shader();

    Shader& use();

    void compile(const char *vs_source, const char *fs_source, const char *geom_source = nullptr);

    void setFloat(const char *name, GLfloat value, bool use_shader = false);
    void setInteger(const char *name, GLint value, bool use_shader = false);
    void setVector2f(const char *name, GLfloat x, GLfloat y, bool use_shader = false);
    void setVector2f(const char *name, const glm::vec2 &value, bool use_shader = false);
    void setVector3f(const char *name, GLfloat x, GLfloat y, GLfloat z, bool use_shader = false);
    void setVector3f(const char *name, const glm::vec3 &value, bool use_shader = false);
    void setVector4f(const char *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, bool use_shader = false);
    void setVector4f(const char *name, const glm::vec4 &value, bool use_shader = false);
    void setMatrix4(const char *name, const glm::mat4 &matrix, bool use_shader = false);

private:
    void checkCompileErrors(GLuint object, const std::string &type);
};


#endif//__GAME_HEADER_GUARD__
