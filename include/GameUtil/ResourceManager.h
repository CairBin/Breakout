#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_
#include <map>
#include <string>
#include "glad/glad.h"
#include "GameUtil/Texture.h"
#include "GameUtil/Shader.h"
#include "GameUtil/Audio.h"

namespace GameUtil{
    class ResourceManager
    {
    public:
        std::map<std::string, Shader> Shaders;
        std::map<std::string, Texture2d> Textures;
        std::map<std::string, Audio> Audios;
        Shader LoadShader(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader, std::string name);
        Shader GetShader(std::string name);
        Texture2d LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
        Texture2d GetTexture(std::string name);

        Audio LoadAudio(const GLchar *file, std::string name);
        Audio GetAudio(std::string name);

        void Clear();
        ResourceManager();

    private:
        
        static Shader loadShaderFromFile(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader = nullptr);
        static Texture2d loadTextureFromFile(const GLchar *file, GLboolean alpha);
    };
}

#endif