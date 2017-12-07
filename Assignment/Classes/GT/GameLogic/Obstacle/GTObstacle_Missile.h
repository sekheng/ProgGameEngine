#ifndef GT_OBSTACLE_MISSILE_H
#define GT_OBSTACLE_MISSILE_H

// Include GT
#include "GTObstacleNode.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacle_Missile : public GTObstacleNode
{
    typedef GTObstacleNode Super;

private:
    MKSprite* m_Rocket = NULL;
    CCParticleSmoke* m_ParticleSmoke = NULL;

protected:
    virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
    GT_INITIALISECONTACTLISTENER(GTObstacle_Missile);
    GT_DEINITIALISECONTACTLISTENER(GTObstacle_Missile);

    static GTObstacle_Missile* Create(MKScene* _scene);

CC_CONSTRUCTOR_ACCESS:
    // Constructor(s) & Destructor
    GTObstacle_Missile(MKScene* _scene) : GTObstacleNode(_scene) {}
    virtual ~GTObstacle_Missile() { UninitialiseContactListener(); }

    virtual bool init() override;
    virtual void update(gtF32 _deltaTime) override;

};

NS_GT_END

#endif // GT_OBSTACLE_MISSILE_H