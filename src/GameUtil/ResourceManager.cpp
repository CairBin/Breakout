#include "GameUtil/ResourceManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "GameUtil/stb_image.h"


namespace GameUtil
{

    ResourceManager::ResourceManager() {}

    Shader ResourceManager::LoadShader(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader, std::string name)
    {
        Shaders[name] = loadShaderFromFile(vShader, fShader, gShader);
        return Shaders[name];
    }

    Shader ResourceManager::GetShader(std::string name)
    {
        return Shaders[name];
    }

    Texture2d ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
    {
        Textures[name] = loadTextureFromFile(file, alpha);
        return Textures[name];
    }

    Texture2d ResourceManager::GetTexture(std::string name)
    {
        return Textures[name];
    }

    void ResourceManager::Clear()
    {
        for (auto iter : Shaders)
            glDeleteProgram(iter.second.Id);
        for (auto iter : Textures)
            glDeleteTextures(1, &iter.second.ID);
    }

    Shader ResourceManager::loadShaderFromFile(const GLchar *vShader, const GLchar *fShader, const GLchar *gShader)
    {
        std::string vexCode;
        std::string fragCode;
        std::string geoCode;
        try
        {
            std::ifstream vexShaderFile(vShader);
            std::ifstream fragShaderFile(fShader);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vexShaderFile.rdbuf();
            fShaderStream << fragShaderFile.rdbuf();
            vexShaderFile.close();
            fragShaderFile.close();
            vexCode = vShaderStream.str();
            fragCode = fShaderStream.str();
            if (gShader != nullptr)
            {
                std::ifstream geoShaderFile(gShader);
                std::stringstream gShaderStream;
                gShaderStream << geoShaderFile.rdbuf();
                geoCode = gShaderStream.str();
            }
        }
        catch (std::exception e)
        {
            std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
        }

        const GLchar *vShaderCode = vexCode.c_str();
        const GLchar *fShaderCode = fragCode.c_str();
        const GLchar *gShaderCode = geoCode.c_str();
        Shader shader;

        shader.Compile(vShaderCode, fShaderCode, gShader != nullptr ? gShaderCode : nullptr);

        return shader;
    }

    Texture2d ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
    {
        Texture2d texture;
        if (alpha)
        {
            texture.Internal_Format = GL_RGBA;
            texture.Image_Format = GL_RGBA;
        }

        GLint width, height, nrChannels;
        unsigned char *data = stbi_load(file, &width, &height, &nrChannels, 0);
        texture.Generate(width, height, data);
        stbi_image_free(data);
        return texture;
    }
}
