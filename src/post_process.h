#ifndef __POST_PROCESS_HEADER_GUARD__
#define __POST_PROCESS_HEADER_GUARD__

#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"
#include "shader.h"


class PostProcess {
public:
    Shader post_processing_shader {};
    Texture2D texture {};

    GLuint width {0}, height {0};
    GLboolean confuse {false}, chaos {false}, shake {false};

    PostProcess(Shader shader, GLuint width, GLuint height);

    void beginRender();
    void endRender();
    void render(GLfloat time);

private:
    void initRenderData();

    GLuint _msfbo;
    GLuint _fbo;
    GLuint _rbo;
    GLuint _vao;

};


#endif//__POST_PROCESS_HEADER_GUARD__
