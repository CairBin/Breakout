#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

#include "GameUtil/GameObject.h"
#include "GameUtil/ResourceManager.h"
#include <vector>

namespace GameUtil{
    class GameLevel{
    public:
        std::vector<GameObject> Bricks;
        GameLevel();
        void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight, ResourceManager *source);
        void Draw(SpriteRenderer &renderer);

        GLboolean IsCompleted();

    private:
        void initialize(std::vector<std::vector<GLuint>> tileData,
                  GLuint levelWidth,
                  GLuint levelHeight,ResourceManager* source);
    };
}

#endif