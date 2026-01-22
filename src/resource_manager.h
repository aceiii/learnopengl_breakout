#pragma once

#include <map>
#include <string>

#include "texture.h"
#include "shader.h"


class ResourceManager {
public:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;

    static Shader loadShader(const char *vs_filename, const char *fs_filename, const char *gs_filename, std::string_view name);
    static Shader getShader(std::string_view name);

    static Texture2D& loadTexture(const char *filename, bool alpha, std::string_view name);
    static Texture2D& getTexture(std::string_view name);

    static void clear();

private:
    ResourceManager();

    static Shader loadShaderFromFile(const char *vs_filename, const char *fs_filename, const char *gs_filename = nullptr);
    static Texture2D loadTextureFromFile(const char *filename, bool alpha);
};
