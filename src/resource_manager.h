#pragma once

#include <map>
#include <string>

#include "texture.h"
#include "shader.h"


class ResourceManager {
public:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;

    static Shader loadShader(std::string_view vs_filename, std::string_view fs_filename, std::string_view gs_filename, std::string_view name);
    static Shader getShader(std::string_view name);

    static Texture2D& loadTexture(std::string_view filename, bool alpha, std::string_view name);
    static Texture2D& getTexture(std::string_view name);

    static void clear();

private:
    ResourceManager();

    static Shader loadShaderFromFile(std::string_view vs_filename, std::string_view fs_filename, std::string_view gs_filename = "");
    static Texture2D loadTextureFromFile(std::string_view, bool alpha);
};
