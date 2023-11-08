#include "Game/Ball.h"

namespace Breakout{

    Ball::Ball()
        : GameObject(), Radius(12.5f), Stuck(true){}

    Ball::Ball(glm::vec2 pos, float radius, glm::vec2 velocity, GameUtil::Texture2d sprite)
        : GameObject(pos, glm::vec2(radius * 2.0f, radius * 2.0f),
                     sprite, glm::vec3(1.0f), velocity),
          Radius(radius), Stuck(true) {}

    glm::vec2 Ball::Move(GLfloat dt,GLuint windowWidth)
    {
        if(!Stuck){
            Position += Velocity * dt;
            if(Position.x <= 0.0f){
                Velocity.x = -Velocity.x;
                Position.x = 0.0f;
            }else if(Position.x+Size.x >=windowWidth){
                Velocity.x = -Velocity.x;
                Position.x = windowWidth - Size.x;
            }

            if(Position.y <= -0.0f){
                Velocity.y = -Velocity.y;
                Position.y = 0.0f;
            }
        }

        return Position;
    }


    void Ball::Reset(glm::vec2 position, glm::vec2 Velocity){
        this->Position = position;
        this->Velocity = Velocity;
        this->Stuck = true;
    }
}