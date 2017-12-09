#include "GTObstacleUI_MissileWarning.h"

NS_GT_BEGIN

const mkString GTObstacle_MissileWarning::m_SpriteFile = "Textures/Gameplay/Obstacles/Missile/Missile_Warning.png";

GTObstacle_MissileWarning* GTObstacle_MissileWarning::Create()
{
    GTObstacle_MissileWarning* sprite = new (std::nothrow) GTObstacle_MissileWarning();
    if (sprite && sprite->initWithFile(m_SpriteFile))
    {
        sprite->autorelease();
        InitGLProgram(sprite);
        InitTextureParameters(sprite);
        return sprite;
    }

    CC_SAFE_DELETE(sprite);
    return nullptr;
}

void GTObstacle_MissileWarning::update(mkF32 _deltaTime)
{

}

NS_GT_END