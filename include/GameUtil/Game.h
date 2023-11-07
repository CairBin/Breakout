#ifndef _GAME_H_
#define _GAME_H_

#include "glad/glad.h"

namespace GameUtil{

    enum class GameState
    {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN
    };

    class Game{
    public:
        static const int MAX_KEYS_SIZE=1024;

    public:
        GameState State;
        GLboolean Keys[MAX_KEYS_SIZE];
        GLuint Width, Height;
        Game(GLuint width, GLuint height);
        ~Game();
        void Initialize();
        void ProcessInput(GLfloat deltaTime);
        void Update(GLfloat deltaTime);
        void Render();
    };
}


#endif