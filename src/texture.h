#ifndef __TEXTURE_HEADER_GUARD__
#define __TEXTURE_HEADER_GUARD__

#include <glad/glad.h>


class Texture2D {
public:
    GLuint id {0};
    GLuint width {0}, height {0};
    GLuint internal_format {0};
    GLuint image_format {0};
    GLuint wrap_s {0}, wrap_t {0};
    GLuint filter_min {0}, filter_max {0};

    Texture2D();
    ~Texture2D();

    void generate(GLuint width, GLuint height, unsigned char *data);
    void bind() const;

};


#endif//__TEXTURE_HEADER_GUARD__
