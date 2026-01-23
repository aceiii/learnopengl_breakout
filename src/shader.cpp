#include <iostream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "gl_check.h"

static void checkCompileErrors(unsigned int object, std::string_view type) {
    int success;
    GLchar info_log[1024];

    if (type != "PROGRAM") {
        GL_CHECK(glGetShaderiv(object, GL_COMPILE_STATUS, &success));
        if (!success) {
            GL_CHECK(glGetShaderInfoLog(object, sizeof(info_log), nullptr, info_log));
            std::println("| ERROR::SHADER: Compile-time error: Type: {}", type);
            std::println("{}", info_log);
            std::println(" -- ---------------------------------------------------- -- ");
        }
    }
    else {
        GL_CHECK(glGetProgramiv(object, GL_LINK_STATUS, &success));
        if (!success) {
            GL_CHECK(glGetProgramInfoLog(object, sizeof(info_log), nullptr, info_log));
            std::println("| ERROR::SHADER: Link-time error: Type: {}", type);
            std::println("{}", info_log);
            std::println(" -- ---------------------------------------------------- -- ");
        }
    }
}

static void compileShader(int &id, GLenum type, std::string_view source, std::string_view debug_name) {
    std::string str {source};
    auto *src = str.c_str();

    GL_CHECK(id = glCreateShader(type));
    GL_CHECK(glShaderSource(id, 1, &src, nullptr));
    GL_CHECK(glCompileShader(id));
    checkCompileErrors(id, debug_name);
}

Shader::Shader() = default;

Shader::~Shader() = default;

void Shader::compile(std::string_view vs_source, std::string_view fs_source, std::string_view geom_source) {
    int shader_vertex, shader_fragment, shader_geom;

    compileShader(shader_vertex, GL_VERTEX_SHADER, vs_source, "VERTEX");
    compileShader(shader_fragment, GL_FRAGMENT_SHADER, fs_source, "FRAGMENT");

    if (!geom_source.empty()) {
        compileShader(shader_geom, GL_GEOMETRY_SHADER, geom_source, "GEOMETRY");
    }

    GL_CHECK(this->id = glCreateProgram());
    GL_CHECK(glAttachShader(this->id, shader_vertex));
    GL_CHECK(glAttachShader(this->id, shader_fragment));
    if (!geom_source.empty()) {
        GL_CHECK(glAttachShader(this->id, shader_geom));
    }

    GL_CHECK(glLinkProgram(this->id));
    checkCompileErrors(this->id, "PROGRAM");

    GL_CHECK(glDeleteShader(shader_vertex));
    GL_CHECK(glDeleteShader(shader_fragment));
    if (!geom_source.empty()) {
        GL_CHECK(glDeleteShader(shader_geom));
    }
}

Shader& Shader::use() {
    GL_CHECK(glUseProgram(this->id));
    return *this;
}

void Shader::setFloat(std::string_view name, float value, bool use_shader) {
    if (use_shader) {
        use();
    }

    int loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, std::string {name}.c_str()));
    GL_CHECK(glUniform1f(loc, value));
}

void Shader::setInteger(std::string_view name, int value, bool use_shader) {
    if (use_shader) {
        use();
    }

    int loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, std::string {name}.c_str()));
    GL_CHECK(glUniform1i(loc, value));
}

void Shader::setVector2f(std::string_view name, float x, float y, bool use_shader) {
    if (use_shader) {
        use();
    }

    int loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, std::string {name}.c_str()));
    GL_CHECK(glUniform2f(loc, x, y));
}

void Shader::setVector2f(std::string_view name, const glm::vec2 &value, bool use_shader) {
    if (use_shader) {
        use();
    }

    int loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, std::string {name}.c_str()));
    GL_CHECK(glUniform2f(loc, value.x, value.y));
}

void Shader::setVector3f(std::string_view name, float x, float y, float z, bool use_shader) {
    if (use_shader) {
        use();
    }

    int loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, std::string {name}.c_str()));
    GL_CHECK(glUniform3f(loc, x, y, z));
}

void Shader::setVector3f(std::string_view name, const glm::vec3 &value, bool use_shader) {
    if (use_shader) {
        use();
    }

    int loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, std::string {name}.c_str()));
    GL_CHECK(glUniform3f(loc, value.x, value.y, value.z));
}

void Shader::setVector4f(std::string_view name, float x, float y, float z, float w, bool use_shader) {
    if (use_shader) {
        use();
    }

    int loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, std::string {name}.c_str()));
    GL_CHECK(glUniform4f(loc, x, y, z, w));
}

void Shader::setVector4f(std::string_view name, const glm::vec4 &value, bool use_shader) {
    if (use_shader) {
        use();
    }

    int loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, std::string {name}.c_str()));
    GL_CHECK(glUniform4f(loc, value.x, value.y, value.z, value.w));
}

void Shader::setMatrix4(std::string_view name, const glm::mat4 &value, bool use_shader) {
    if (use_shader) {
        use();
    }

    int loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, std::string {name}.c_str()));
    GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
}
