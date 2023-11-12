#include "Game/PowerUp.h"

namespace Breakout{
    const glm::vec2 SIZE(60, 20);
    const glm::vec2 VELOCITY(0.0f, 150.0f);
    PowerUp::PowerUp(PowerUpType type, glm::vec3 color,
            GLfloat duration, glm::vec2 position, GameUtil::Texture2d texture)
        : GameObject(position, SIZE, texture, color, VELOCITY),
          Type(type), Duration(duration), Activated() {}
}