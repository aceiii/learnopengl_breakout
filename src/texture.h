#pragma once


class Texture2D {
public:
    unsigned int id {0};
    unsigned int width {0}, height {0};
    unsigned int internal_format {0};
    unsigned int image_format {0};
    unsigned int wrap_s {0}, wrap_t {0};
    unsigned int filter_min {0}, filter_max {0};

    Texture2D();

    void generate(unsigned int width, unsigned int height, unsigned char *data);
    void bind() const;
};
