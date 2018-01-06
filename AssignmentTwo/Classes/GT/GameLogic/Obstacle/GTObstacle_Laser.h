#ifndef GT_OBSTACLE_LASER_H
#define GT_OBSTACLE_LASER_H

// Include GT
#include "GTObstacleNode.h"
#include "../../../GT/Audio/GTSimperMusicSys.h"
#include "../../../GT/Actions/GTRemoveFromParentAction.h"
#include "../../../GT/Actions/GTScaleUpAndDownAction.h"
#include "../../../GT/Actions/GTPlaySoundAction.h"
#include "../../../GT/Actions/GTFollowNodeAction.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacle_Laser : public GTObstacleNode
{
    typedef GTObstacleNode Super;

protected:
    MKSprite* m_LaserGunLeft = NULL;
    MKSprite* m_LaserGunRight = NULL;
    gtS32 m_LaserGunZPriority = 1;
    gtS32 m_LaserBeamZPriority = 0;

    virtual void SpawnParticles();
    virtual void SpawnLaserBeam();
    virtual void SpawnPhysicsBody();
    virtual void DespawnPhysicsBody();
    virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
    // Sprite
    static const mkString m_LaserGunSpriteFile;
    static const mkString m_BeamPListFile;
    static const mkString m_BeamJSONFile;
    static const mkString m_BeamSpriteFrameName;
    static const mkString m_BeamTransitState;

    // Audio
    static const mkString m_LaserChargingSoundName;
    static const mkString m_LaserShootingSoundName;

    // Others
    static const gtF32 m_MoveDownDuration;
    static const gtF32 m_LaserBeamChargeDuration;
    static const gtF32 m_LaserBeamShootDuration;
    static const gtF32 m_MoveUpDuration;
    static const gtF32 m_TotalDuration;

    static GTObstacle_Laser* Create(MKScene* _scene);

    GT_INITIALISECONTACTLISTENER(GTObstacle_Laser);
    GT_DEINITIALISECONTACTLISTENER(GTObstacle_Laser);

CC_CONSTRUCTOR_ACCESS:
    // Constructor(s) & Destructor
    GTObstacle_Laser(MKScene* _scene) : GTObstacleNode(_scene) {}
    virtual ~GTObstacle_Laser()
    {
        DeinitialiseContactListener();
    }

    virtual gtBool init();
    virtual void update(gtF32 _deltaTime) override {}
};

NS_GT_END

#endif // GT_OBSTACLE_LASER_H