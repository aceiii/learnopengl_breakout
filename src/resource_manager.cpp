#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "resource_manager.h"
#include "gl_check.h"


std::map<std::string, Shader> ResourceManager::shaders;
std::map<std::string, Texture2D> ResourceManager::textures;

ResourceManager::ResourceManager() = default;

void ResourceManager::clear() {
    for (const auto& it : shaders) {
        GL_CHECK(glDeleteProgram(it.second.id));
    }

    for (const auto& it : textures) {
        GL_CHECK(glDeleteTextures(1, &it.second.id));
    }
}

Shader ResourceManager::loadShader(std::string_view vs_filename, std::string_view fs_filename, std::string_view gs_filename, std::string_view name) {
    std::string key(name);
    shaders[key] = loadShaderFromFile(vs_filename, fs_filename, gs_filename);
    return shaders[key];
}

Shader ResourceManager::getShader(std::string_view name) {
    return shaders[std::string(name)];
}

Texture2D& ResourceManager::loadTexture(std::string_view filename, bool alpha, std::string_view name) {
    std::string key(name);
    textures[key] = loadTextureFromFile(filename, alpha);
    return textures[key];
}

Texture2D& ResourceManager::getTexture(std::string_view name) {
    return textures[std::string(name)];
}

Shader ResourceManager::loadShaderFromFile(std::string_view vs_filename, std::string_view fs_filename, std::string_view gs_filename) {
    std::println("ResourceManager::loadFromFile: '{}', '{}', '{}'", vs_filename, fs_filename, gs_filename);

    std::string vertex_code;
    std::string fragment_code;
    std::string geometry_code;

    try {
        std::ifstream vs_file {std::filesystem::path{vs_filename}};
        std::ifstream fs_file {std::filesystem::path{fs_filename}};

        if (!vs_file) {
            std::println("ERROR::SHADER: Failed to read file '{}'", vs_filename);
        }

        if (!fs_file) {
            std::println("ERROR::SHADER: Failed to read file '{}'", fs_filename);
        }

        std::stringstream vs_stream, fs_stream;

        vs_stream << vs_file.rdbuf();
        fs_stream << fs_file.rdbuf();

        vs_file.close();
        fs_file.close();

        vertex_code = vs_stream.str();
        fragment_code = fs_stream.str();

        if (!gs_filename.empty()) {
            std::ifstream gs_file {std::filesystem::path(gs_filename)};
            std::stringstream gs_stream;

            gs_stream << gs_file.rdbuf();
            gs_file.close();

            geometry_code = gs_stream.str();
        }
    }
    catch (std::exception& e) {
        std::println("ERROR::SHADER: Failed to read shader files");
    }

    Shader shader;
    shader.compile(vertex_code, fragment_code, geometry_code);

    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(std::string_view filename, bool alpha) {
    Texture2D texture;
    if (alpha) {
        texture.internal_format = GL_RGBA;
        texture.image_format = GL_RGBA;
    }

    int width, height;
    unsigned char *image = stbi_load(std::string {filename}.c_str(), &width, &height, nullptr, texture.image_format == GL_RGBA ? STBI_rgb_alpha : STBI_rgb);

    texture.generate(width, height, image);

    stbi_image_free(image);

    return texture;
}

