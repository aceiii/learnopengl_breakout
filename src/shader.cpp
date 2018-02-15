#include "shader.h"
#include "gl_check.h"

Shader::Shader() {
}

Shader::~Shader() {
}

void Shader::compile(const char *vs_source, const char *fs_source, const char *geom_source) {
    GLuint shader_vertex, shader_fragment, shader_geom;

    GL_CHECK(shader_vertex = glCreateShader(GL_VERTEX_SHADER));
    GL_CHECK(glShaderSource(shader_vertex, 1, &vs_source, nullptr));
    GL_CHECK(glCompileShader(shader_vertex));
    checkCompileErrors(shader_vertex, "VERTEX");

    GL_CHECK(shader_fragment = glCreateShader(GL_FRAGMENT_SHADER));
    GL_CHECK(glShaderSource(shader_fragment, 1, &fs_source, nullptr));
    GL_CHECK(glCompileShader(shader_fragment));
    checkCompileErrors(shader_fragment, "FRAGMENT");

    if (geom_source) {
        GL_CHECK(shader_geom = glCreateShader(GL_GEOMETRY_SHADER));
        GL_CHECK(glShaderSource(shader_geom, 1, &geom_source, nullptr));
        GL_CHECK(glCompileShader(shader_geom));
        checkCompileErrors(shader_geom, "GEOMETRY");
    }

    GL_CHECK(this->id = glCreateProgram());
    GL_CHECK(glAttachShader(this->id, shader_vertex));
    GL_CHECK(glAttachShader(this->id, shader_fragment));
    if (geom_source) {
        GL_CHECK(glAttachShader(this->id, shader_geom));
    }

    GL_CHECK(glLinkProgram(this->id));
    checkCompileErrors(this->id, "PROGRAM");

    GL_CHECK(glDeleteShader(shader_vertex));
    GL_CHECK(glDeleteShader(shader_fragment));
    if (geom_source) {
        GL_CHECK(glDeleteShader(shader_geom));
    }
}

Shader& Shader::use() {
    GL_CHECK(glUseProgram(this->id));
    return *this;
}

void Shader::setFloat(const char *name, GLfloat value, bool use_shader) {
    if (use_shader) {
        use();
    }

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, name));
    GL_CHECK(glUniform1f(loc, value));
}

void Shader::setInteger(const char *name, GLint value, bool use_shader) {
    if (use_shader) {
        use();
    }

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, name));
    GL_CHECK(glUniform1i(loc, value));
}

void Shader::setVector2f(const char *name, GLfloat x, GLfloat y, bool use_shader) {
    if (use_shader) {
        use();
    }

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, name));
    GL_CHECK(glUniform2f(loc, x, y));
}

void Shader::setVector2f(const char *name, const glm::vec2 &value, bool use_shader) {
    if (use_shader) {
        use();
    }

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, name));
    GL_CHECK(glUniform2f(loc, value.x, value.y));
}

void Shader::setVector3f(const char *name, GLfloat x, GLfloat y, GLfloat z, bool use_shader) {
    if (use_shader) {
        use();
    }

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, name));
    GL_CHECK(glUniform3f(loc, x, y, z));
}

void Shader::setVector3f(const char *name, const glm::vec3 &value, bool use_shader) {
    if (use_shader) {
        use();
    }

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, name));
    GL_CHECK(glUniform3f(loc, value.x, value.y, value.z));
}

void Shader::setVector4f(const char *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, bool use_shader) {
    if (use_shader) {
        use();
    }

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, name));
    GL_CHECK(glUniform4f(loc, x, y, z, w));
}

void Shader::setVector4f(const char *name, const glm::vec4 &value, bool use_shader) {
    if (use_shader) {
        use();
    }

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, name));
    GL_CHECK(glUniform4f(loc, value.x, value.y, value.z, value.w));
}

void Shader::setMatrix4(const char *name, const glm::mat4 &value, bool use_shader) {
    if (use_shader) {
        use();
    }

    GLint loc;
    GL_CHECK(loc = glGetUniformLocation(this->id, name));
    GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value)));
}

void Shader::checkCompileErrors(GLuint object, const std::string &type) {
    GLint success;
    GLchar info_log[1024];

    if (type != "PROGRAM") {
        GL_CHECK(glGetShaderiv(object, GL_COMPILE_STATUS, &success));
        if (!success) {
            GL_CHECK(glGetShaderInfoLog(object, sizeof(info_log), nullptr, info_log));
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << '\n'
                << info_log << "\n -- ---------------------------------------------------- -- \n";
        }
    }
    else {
        GL_CHECK(glGetShaderiv(object, GL_LINK_STATUS, &success));
        if (!success) {
            GL_CHECK(glGetShaderInfoLog(object, sizeof(info_log), nullptr, info_log));
            std::cout << "| ERROR::SHADER: Link-time error: Type: " << type << '\n'
                << info_log << "\n -- ---------------------------------------------------- -- \n";
        }
    }
}

