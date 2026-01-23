#include <memory>
#include <print>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "util.h"
#include "game.h"
#include "resource_manager.h"
#include "gl_check.h"

namespace {
    constexpr unsigned int SCREEN_WIDTH = 800;
    constexpr unsigned int SCREEN_HEIGHT = 600;
    std::unique_ptr<Game> breakout {nullptr};
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

int main() {

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true); // Required on osx or crash
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);

    float fb_scale = std::min(static_cast<float>(fb_width) / SCREEN_WIDTH, static_cast<float>(fb_height) / SCREEN_HEIGHT);

    breakout = std::make_unique<Game>(SCREEN_WIDTH, SCREEN_HEIGHT, fb_scale);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::println("Failed to initialize GLAD");
        return -1;
    }

    glGetError();

    glfwSetKeyCallback(window, keyCallback);

    GL_CHECK(glViewport(0, 0, SCREEN_WIDTH * fb_scale, SCREEN_HEIGHT * fb_scale));
    GL_CHECK(glEnable(GL_CULL_FACE));
    GL_CHECK(glEnable(GL_BLEND));
    GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    breakout->init();

    double delta_time = 0.0f;
    double last_frame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        const auto current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        glfwPollEvents();

        const auto dt = static_cast<float>(delta_time);
        breakout->processInput(dt);
        breakout->update(dt);

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
        glfwSetWindowShouldClose(window, true);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            breakout->keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            breakout->keys[key] = false;
            breakout->keys_processed[key] = false;
        }
    }
}

