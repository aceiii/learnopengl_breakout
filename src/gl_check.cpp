#include <iostream>
#include <unordered_map>
#include <glad/glad.h>

#include "gl_check.h"


namespace {
    std::unordered_map<GLenum, const char*> error_map = {
        {GL_NO_ERROR, "GL_NO_ERROR"},
        {GL_INVALID_ENUM, "GL_INVALID_ENUM"},
        {GL_INVALID_VALUE, "GL_INVALID_VALUE"},
        {GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
        {GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
        {GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
        {GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
        {GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"},
    };
}

void checkOpenglError(const char *statement, const char *file, int line) {

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::println("ERROR: {} ({})", error_map[error], error);
        std::println("     > {}:{}", file, line);
        std::println("     > `{}", statement);
        exit(1);
    }
}

