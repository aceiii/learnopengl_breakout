#pragma once

#include <string_view>
#include <glm/glm.hpp>


class Shader {
public:
    unsigned int id {0};

    Shader();
    ~Shader();

    Shader& use();

    void compile(std::string_view vs_source, std::string_view fs_source, std::string_view geom_source = {});

    void setFloat(std::string_view name, float value, bool use_shader = false);
    void setInteger(std::string_view name, int value, bool use_shader = false);
    void setVector2f(std::string_view name, float x, float y, bool use_shader = false);
    void setVector2f(std::string_view name, const glm::vec2 &value, bool use_shader = false);
    void setVector3f(std::string_view name, float x, float y, float z, bool use_shader = false);
    void setVector3f(std::string_view name, const glm::vec3 &value, bool use_shader = false);
    void setVector4f(std::string_view name, float x, float y, float z, float w, bool use_shader = false);
    void setVector4f(std::string_view name, const glm::vec4 &value, bool use_shader = false);
    void setMatrix4(std::string_view name, const glm::mat4 &matrix, bool use_shader = false);
};
