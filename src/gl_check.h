#pragma once


#define GL_CHECK(stmt) do {\
        stmt; \
        checkOpenglError(#stmt, __FILE__, __LINE__); \
    } while (0)

void checkOpenglError(const char *statement, const char *filename, int line);
