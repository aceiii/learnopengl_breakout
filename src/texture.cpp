#include "texture.h"
#include "gl_check.h"

Texture2D::Texture2D()
    : width {0}, height {0},
      internal_format {GL_RGB}, image_format {GL_RGB},
      wrap_s {GL_REPEAT}, wrap_t {GL_REPEAT},
      filter_min {GL_LINEAR}, filter_max {GL_LINEAR} { }

Texture2D::~Texture2D() {
}

void Texture2D::generate(GLuint width_, GLuint height_, unsigned char *data) {
    width = width_;
    height = height_;

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->id));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, data));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::bind() const {
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, this->id));
}

