#include "gl_check.h"
#include <iostream>

void checkOpenglError(const char *statement, const char *file, int line) {

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "ERROR: (" << error << ") " << file << ':' << line << '\n';
        std::cout << "  `" << statement << "`\n";
        exit(1);
    }
}

