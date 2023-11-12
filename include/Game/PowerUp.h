#ifndef _POWER_UP_H_
#define _POWER_UP_H_
#include "GameUtil/GameObject.h"
#include "GameUtil/Texture.h"

namespace Breakout{
    enum class PowerUpType
    {
        SPEED,              //速度
        STICKY,             //黏住
        PASS_THROUGH,       //穿越
        PAD_SIZE_INCREASE,  //增加板子长度
    /*********** Debuff *************/
        CONFUSE,    //颠倒+反色
        CHAOS       //混乱
    };

    class PowerUp:public GameUtil::GameObject{
    public:
        PowerUpType Type;   //道具类型
        GLfloat Duration;
        GLboolean Activated;

        PowerUp(PowerUpType type, glm::vec3 color,
                GLfloat duration, glm::vec2 position, GameUtil::Texture2d texture);
    };
}


#endif