#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <SOIL.h>

std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;

ResourceManager::ResourceManager() {
}

void ResourceManager::clear() {
    for (auto it : shaders) {
        GL_CHECK(glDeleteProgram(it.second.id));
    }

    for (auto it : textures) {
        GL_CHECK(glDeleteTextures(1, &it.second.id));
    }
}

Shader ResourceManager::loadShader(const char *vs_filename, const char *fs_filename, const char *gs_filename, const std::string &name) {
    shaders[name] = loadShaderFromFile(vs_filename, fs_filename, gs_filename);
    return shaders[name];
}

Shader ResourceManager::getShader(const std::string &name) {
    return shaders[name];
}

Texture2D& ResourceManager::loadTexture(const char *filename, bool alpha, const std::string &name) {
    textures[name] = loadTextureFromFile(filename, alpha);
    return textures[name];
}

Texture2D& ResourceManager::getTexture(const std::string &name) {
    return textures[name];
}

Shader ResourceManager::loadShaderFromFile(const char *vs_filename, const char *fs_filename, const char *gs_filename) {
    std::string vertex_code;
    std::string fragment_code;
    std::string geometry_code;

    try {
        std::ifstream vs_file {vs_filename};
        std::ifstream fs_file {fs_filename};

        if (!vs_file) {
            std::cout << "ERROR::SHADER: Failed to read file '" << vs_filename << "'\n";
        }

        if (!fs_file) {
            std::cout << "ERROR::SHADER: Failed to read file '" << fs_filename << "'\n";
        }

        std::stringstream vs_stream, fs_stream;

        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();

        vs_file.close();
        fs_file.close();

        vertex_code = vs_stream.str();
        fragment_code = fs_stream.str();

        if (gs_filename) {
            std::ifstream gs_file {gs_filename};
            std::stringstream gs_stream;

            gs_stream << gs_file.rdbuf();
            gs_file.close();

            geometry_code = gs_stream.str();
        }
    }
    catch (std::exception e) {
        std::cout << "ERROR::SHADER: Failed to read shader files\n";
    }

    const GLchar *vs_source = vertex_code.c_str();
    const GLchar *fs_source = fragment_code.c_str();
    const GLchar *gs_source = geometry_code.c_str();

    Shader shader;
    shader.compile(vs_source, fs_source, gs_filename ? gs_source : nullptr);

    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char *filename, bool alpha) {
    Texture2D texture;
    if (alpha) {
        texture.internal_format = GL_RGBA;
        texture.image_format = GL_RGBA;
    }

    int width, height;
    unsigned char *image = SOIL_load_image(filename, &width, &height, 0, texture.image_format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

    texture.generate(width, height, image);

    SOIL_free_image_data(image);

    return texture;
}

