#ifndef GT_OBSTACLEUI_MISSILEWARNING_H
#define GT_OBSTACLEUI_MISSILEWARNING_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "../../../Common/GTMacros.h"

// Include MK
#include "../../../../MK/Common/MKMacros.h"
#include "../../../../MK/Graphics/MKSprite.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacle_MissileWarning : public MKSprite
{
private:
    static const mkString m_SpriteFile;

    mkF32 m_Lifetime;
    mkF32 m_ScaleSpeed = 5.0f;

protected:

public:
    static GTObstacle_MissileWarning* Create();

    virtual void update(mkF32 _deltaTime) override;

CC_CONSTRUCTOR_ACCESS:
    GTObstacle_MissileWarning(mkF32 _lifetime) : MKSprite(false), m_Lifetime(_lifetime) {}
    virtual ~GTObstacle_MissileWarning() {}

};

NS_GT_END

#endif // GT_OBSTACLEUI_MISSILEWARNING_H