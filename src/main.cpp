#include <iostream>
#include <memory>

#include "util.h"
#include "game.cpp"
#include "shader.cpp"
#include "texture.cpp"
#include "gl_check.cpp"
#include "sprite_renderer.cpp"
#include "resource_manager.cpp"
#include "game_object.cpp"
#include "game_level.cpp"
#include "ball_object.cpp"
#include "particle_generator.cpp"
#include "post_process.cpp"
#include "powerup.cpp"
#include "mp3audio.cpp"
#include "text_renderer.cpp"


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

const GLuint SCREEN_WIDTH = 800;
const GLuint SCREEN_HEIGHT = 600;

std::unique_ptr<Game> breakout {nullptr};

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on osx or crash
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

    GLfloat fb_scale = std::min(fb_width / GLfloat(SCREEN_WIDTH), fb_height / GLfloat(SCREEN_HEIGHT));

    breakout = std::make_unique<Game>(SCREEN_WIDTH, SCREEN_HEIGHT, fb_scale);

    glewExperimental = GL_TRUE;
    glewInit();
    glGetError();

    glfwSetKeyCallback(window, keyCallback);

    GL_CHECK(glViewport(0, 0, SCREEN_WIDTH * fb_scale, SCREEN_HEIGHT * fb_scale));
    GL_CHECK(glEnable(GL_CULL_FACE));
    GL_CHECK(glEnable(GL_BLEND));
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    breakout->init();

    GLfloat delta_time = 0.0f;
    GLfloat last_frame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        GLfloat current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glfwPollEvents();

        breakout->processInput(delta_time);
        breakout->update(delta_time);

        GL_CHECK(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));

        breakout->render();

        glfwSwapBuffers(window);
    }

    ResourceManager::clear();

    glfwTerminate();

    return 0;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    UNUSED(scancode);
    UNUSED(mode);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            breakout->keys[key] = GL_TRUE;
        }
        else if (action == GLFW_RELEASE) {
            breakout->keys[key] = GL_FALSE;
            breakout->keys_processed[key] = GL_FALSE;
        }
    }
}

