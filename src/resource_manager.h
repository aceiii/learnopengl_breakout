#ifndef __RESOURCE_MANAGER_HEADER_GUARD__
#define __RESOURCE_MANAGER_HEADER_GUARD__

#include <map>
#include <string>

#include "texture.h"
#include "shader.h"


class ResourceManager {
public:
    static std::map<std::string, Shader> shaders;
    static std::map<std::string, Texture2D> textures;

    static Shader loadShader(const char *vs_filename, const char *fs_filename, const char *gs_filename, const std::string &name);
    static Shader getShader(const std::string &name);

    static Texture2D& loadTexture(const char *filename, bool alpha, const std::string &name);
    static Texture2D& getTexture(const std::string &name);

    static void clear();

private:
    ResourceManager();

    static Shader loadShaderFromFile(const char *vs_filename, const char *fs_filename, const char *gs_filename = nullptr);
    static Texture2D loadTextureFromFile(const char *filename, bool alpha);
};


#endif//__RESOURCE_MANAGER_HEADER_GUARD__
