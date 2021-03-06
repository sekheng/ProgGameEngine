#ifndef GT_OBSTACLE_SAW_H
#define GT_OBSTACLE_SAW_H

// Include GT
#include "GTObstacleNode.h"
#include "../../../GT/Audio/GTSimperMusicSys.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacle_Saw : public GTObstacleNode
{
	typedef GTObstacleNode Super;

protected:
	MKSprite* m_Saw = NULL;
    gtF32 m_RotationSpeed = 720.0f;
    gtS32 m_SawSpinningSoundID = GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND;

	virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
	// Sprite
	static const mkString m_SawSpriteFile;
    static const mkString m_ExplosionPListFile;
    static const mkString m_ExplosionJSONFile;
    static const mkString m_ExplosionSpriteFrameName;
    static const mkString m_ExplosionTransitState;

    // Audio
    static const mkString m_SawSpinningSoundName;
    static const mkString m_SawHitSoundName;
    static const mkString m_SawExplosionSoundName;

	static GTObstacle_Saw* Create(MKScene* _scene);

	GT_INITIALISECONTACTLISTENER(GTObstacle_Saw);
	GT_DEINITIALISECONTACTLISTENER(GTObstacle_Saw);

    virtual void PauseObstacle()
    {
        Super::PauseObstacle();

        // Pause Audio
        if (m_SawSpinningSoundID != GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
        {
            GTSimperMusicSys::GetInstance()->pauseSound(m_SawSpinningSoundID);
        }

        // Pause Actions
        _actionManager->pauseTarget(this);
    }
    virtual void ResumeObstacle()
    {
        Super::ResumeObstacle();

        // Resume Audio
        if (m_SawSpinningSoundID != GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
        {
            GTSimperMusicSys::GetInstance()->resumeSound(m_SawSpinningSoundID);
        }

        // Resume Actions
        _actionManager->resumeTarget(this);
    }

    virtual void DestroyObstacle() override;

CC_CONSTRUCTOR_ACCESS:
	// Constructor(s) & Destructor
	GTObstacle_Saw(MKScene* _scene) : GTObstacleNode(_scene)
    {
        m_DestroyedAnimationDuration = 0.28f;
    }
	virtual ~GTObstacle_Saw()
	{
		DeinitialiseContactListener();
        if (m_SawSpinningSoundID != GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
        {
            GTSimperMusicSys::GetInstance()->stopSound(m_SawSpinningSoundID);
        }
	}

    virtual void update(gtF32 _deltaTime) override;
	virtual gtBool init() override;
};

NS_GT_END

#endif // GT_OBSTACLE_MISSILE_H
