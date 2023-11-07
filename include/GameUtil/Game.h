#ifndef _GAME_H_
#define _GAME_H_

#include "glad/glad.h"
#include <string>
#include <vector>
#include "GameUtil/GameLevel.h"
#include "GameUtil/ResourceManager.h"

namespace GameUtil{
    enum class GameState
    {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN
    };

    class Game{
    protected:
        ResourceManager* SourceManager;

    public:
        static const int MAX_KEYS_SIZE=1024;

    public:
        std::string ResourcePath;
        GameState State;
        GLboolean Keys[MAX_KEYS_SIZE];
        GLuint Width, Height;
        Game(GLuint width, GLuint height,std::string resourcePath);
        ~Game();
        std::string GetResourceFilePath(const std::string& filename);
        virtual void Initialize() = 0;
        virtual void ProcessInput(GLfloat deltaTime) = 0;
        virtual void Update(GLfloat deltaTime) = 0;
        virtual void Render()=0;
        void SetResourceManager(ResourceManager& sourceManager);
    };

    Game *GetGame();
}


#endif