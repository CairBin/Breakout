#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "GameUtil/GameObject.h"
#include "GameUtil/Direction.h"

namespace GameUtil{

class Collision{
public:
    typedef std::tuple<GLboolean, DirectState2d, glm::vec2> CollisionResult;

public:
    static GLboolean AabbCheckCollision(GameObject &a, GameObject &b);
};
}


#endif