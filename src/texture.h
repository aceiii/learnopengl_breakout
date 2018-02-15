#ifndef __TEXTURE_HEADER_GUARD__
#define __TEXTURE_HEADER_GUARD__

#include <GL/glew.h>


class Texture2D {
public:
    GLuint id;
    GLuint width, height;
    GLuint internal_format;
    GLuint image_format;
    GLuint wrap_s, wrap_t;
    GLuint filter_min, filter_max;

    Texture2D();
    ~Texture2D();

    void generate(GLuint width, GLuint height, unsigned char *data);
    void bind() const;

};


#endif//__TEXTURE_HEADER_GUARD__
