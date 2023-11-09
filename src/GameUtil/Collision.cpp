#include "GameUtil/Collision.h"

namespace GameUtil
{
    GLboolean Collision::AabbCheckCollision(GameObject &a, GameObject &b)
    {
        bool collX = a.Position.x + a.Size.x >= b.Position.x && b.Position.x + b.Size.x >= a.Position.x;
        bool collY = a.Position.y + a.Size.y >= b.Position.y && b.Position.y + b.Size.y >= a.Position.y;

        return collX && collY;
    }
}