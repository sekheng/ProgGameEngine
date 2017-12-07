#ifndef GT_OBSTACLE_MISSILE_H
#define GT_OBSTACLE_MISSILE_H

// Include GT
#include "GTObstacleNode.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacle_Missile : public GTObstacleNode
{
    typedef GTObstacleNode Super;

protected:
    virtual bool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
    Vec2 m_Speed = Vec2(-500.0f, 0.0f);

    GT_REGISTERCONTACTCALLBACK(GTObstacle_Missile);

CC_CONSTRUCTOR_ACCESS:
    // Constructor(s) & Destructor
    GTObstacle_Missile() {}
    virtual ~GTObstacle_Missile() {}

    virtual bool init() override;
    virtual void update(float _deltaTime) override;

};

NS_GT_END

#endif // GT_OBSTACLE_MISSILE_H