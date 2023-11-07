#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_
#include <map>
#include <string>
#include "glad/glad.h"
#include "GameUtil/Texture.h"
#include "GameUtil/Shader.h"

namespace GameUtil{
    class ResourceManager
    {
    public:
        static std::map<std::string, Shader> Shaders;
        static std::map<std::string, Texture2d> Textures;
        static Shader LoadShader(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader, std::string name);
        static Shader GetShader(std::string name);
        static Texture2d LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
        static Texture2d GetTexture(std::string name);
        static void Clear();

    private:
        ResourceManager();
        static Shader loadShaderFromFile(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader = nullptr);
        static Texture2d loadTextureFromFile(const GLchar *file, GLboolean alpha);
    };
}

#endif