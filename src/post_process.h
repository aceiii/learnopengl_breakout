#pragma once

#include "texture.h"
#include "shader.h"


class PostProcess {
public:
    Shader post_processing_shader {};
    Texture2D texture {};

    unsigned int width {0}, height {0};
    bool confuse {false}, chaos {false}, shake {false};

    PostProcess(Shader shader, unsigned int width, unsigned int height);

    void beginRender();
    void endRender();
    void render(float time);

private:
    void initRenderData();

    unsigned int _msfbo{};
    unsigned int _fbo{};
    unsigned int _rbo{};
    unsigned int _vao{};

};
