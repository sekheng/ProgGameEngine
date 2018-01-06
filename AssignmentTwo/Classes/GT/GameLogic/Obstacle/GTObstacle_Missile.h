#ifndef GT_OBSTACLE_MISSILE_H
#define GT_OBSTACLE_MISSILE_H

// Include GT
#include "GTObstacleNode.h"
#include "../../../GT/Audio/GTSimperMusicSys.h"
#include "../../../GT/Actions/GTRemoveFromParentAction.h"
#include "../../../GT/Actions/GTScaleUpAndDownAction.h"
#include "../../../GT/Actions/GTPlaySoundAction.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacle_Missile : public GTObstacleNode
{
    typedef GTObstacleNode Super;

protected:
    MKSprite* m_Missile = NULL;
    CCParticleSmoke* m_ParticleSmoke = NULL;
    gtS32 m_MissileFlightSoundID = GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND;
    gtBool m_SpawnedWarning = false;

    virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
    // Sprite
    static const mkString m_MissileSpriteFile;
    static const mkString m_ExplosionPListFile;
    static const mkString m_ExplosionJSONFile;
    static const mkString m_ExplosionSpriteFrameName;
    static const mkString m_ExplosionTransitState;
    static const mkString m_MissileWarningSpriteFile;

    // Audio
    static const mkString m_MissileFlightSoundName;
    static const mkString m_MissileExplosionSoundName;
    static const mkString m_MissileWarningSoundName;

    // Others
    static const gtF32 m_WarningDuration;

    static GTObstacle_Missile* Create(MKScene* _scene);
    static gtF32 GetHorizontalVelocity();
    static MKSprite* CreateMissileWarning();

    GT_INITIALISECONTACTLISTENER(GTObstacle_Missile);
    GT_DEINITIALISECONTACTLISTENER(GTObstacle_Missile);

    virtual void PauseObstacle()
    {
        Super::PauseObstacle();

        // Pause Audio
        if (m_MissileFlightSoundID != GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
        {
            GTSimperMusicSys::GetInstance()->pauseSound(m_MissileFlightSoundID);
        }

        // Pause Actions
        _actionManager->pauseTarget(this);
    } 
    virtual void ResumeObstacle()
    {
        Super::ResumeObstacle();

        // Resume Audio
        if (m_MissileFlightSoundID != GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
        {
            GTSimperMusicSys::GetInstance()->resumeSound(m_MissileFlightSoundID);
        }

        // Resume Actions
        _actionManager->resumeTarget(this);
    }

CC_CONSTRUCTOR_ACCESS:
    // Constructor(s) & Destructor
    GTObstacle_Missile(MKScene* _scene) : GTObstacleNode(_scene) {}
    virtual ~GTObstacle_Missile()
    {
        DeinitialiseContactListener();
        if (m_MissileFlightSoundID != GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
        {
            GTSimperMusicSys::GetInstance()->stopSound(m_MissileFlightSoundID);
        }
    }

    virtual gtBool init() override;
    virtual void update(gtF32 _deltaTime) override;

};

NS_GT_END

#endif // GT_OBSTACLE_MISSILE_H