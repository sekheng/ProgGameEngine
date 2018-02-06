#ifndef GT_OBSTACLE_SPIKE_H
#define GT_OBSTACLE_SPIKE_H

// Include GT
#include "GTObstacleNode.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacle_Spike : public GTObstacleNode
{
	typedef GTObstacleNode Super;

protected:
	MKSprite* m_Spike = NULL;
	gtU32 m_NumberOfSpikes = 2;

	virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
    // Sprite
    static const mkString m_SpikeSpriteFile;
    static const mkString m_ExplosionPListFile;
    static const mkString m_ExplosionJSONFile;
    static const mkString m_ExplosionSpriteFrameName;
    static const mkString m_ExplosionTransitState;

    // Audio
    static const mkString m_SpikeHitSoundName;
    static const mkString m_SpikeExplosionSoundName;

	GT_INITIALISECONTACTLISTENER(GTObstacle_Spike);
	GT_DEINITIALISECONTACTLISTENER(GTObstacle_Spike);

	static GTObstacle_Spike* Create(MKScene* _scene, gtU32 _numberOfSpikes = 2);

    virtual void DestroyObstacle() override;

CC_CONSTRUCTOR_ACCESS:
	// Constructor(s) & Destructor
	GTObstacle_Spike(MKScene* _scene, gtU32 _numberOfSpikes) :
        GTObstacleNode(_scene),
        m_NumberOfSpikes(_numberOfSpikes)
    {
        m_DestroyedAnimationDuration = 0.28f;
    }
	virtual ~GTObstacle_Spike()
	{
		DeinitialiseContactListener();
	}

	virtual bool init();

};

NS_GT_END

#endif // GT_OBSTACLE_MISSILE_H
