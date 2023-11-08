#ifndef _BALL_H_
#define _BALL_H_

#include "GameUtil/GameObject.h"
#include "GameUtil/Texture.h"

namespace Breakout{
    class Ball:public GameUtil::GameObject{
    public:
        GLfloat Radius;
        GLboolean Stuck;
        Ball();
        Ball(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, GameUtil::Texture2d sprite);

        glm::vec2 Move(GLfloat dt, GLuint widnowWidth);
        void Reset(glm::vec2 position, glm::vec2 velocity);
    };
}

#endif