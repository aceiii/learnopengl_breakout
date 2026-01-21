#ifndef __GL_CHECK_HEADER_GUARD__
#define __GL_CHECK_HEADER_GUARD__


#define GL_CHECK(stmt) do {\
        stmt; \
        checkOpenglError(#stmt, __FILE__, __LINE__); \
    } while (0)

void checkOpenglError(const char *statement, const char *filename, int line);


#endif//__GL_CHECK_HEADER_GUARD__
