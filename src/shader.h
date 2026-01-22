#pragma once

#include <string_view>
#include <glm/glm.hpp>


class Shader {
public:
    unsigned int id {0};

    Shader();
    ~Shader();

    Shader& use();

    void compile(const char *vs_source, const char *fs_source, const char *geom_source = "");

    void setFloat(const char *name, float value, bool use_shader = false);
    void setInteger(const char *name, int value, bool use_shader = false);
    void setVector2f(const char *name, float x, float y, bool use_shader = false);
    void setVector2f(const char *name, const glm::vec2 &value, bool use_shader = false);
    void setVector3f(const char *name, float x, float y, float z, bool use_shader = false);
    void setVector3f(const char *name, const glm::vec3 &value, bool use_shader = false);
    void setVector4f(const char *name, float x, float y, float z, float w, bool use_shader = false);
    void setVector4f(const char *name, const glm::vec4 &value, bool use_shader = false);
    void setMatrix4(const char *name, const glm::mat4 &matrix, bool use_shader = false);

private:
    void checkCompileErrors(unsigned int object, std::string_view type);
};
