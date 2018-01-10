#ifndef GT_CLEAN_SLATE_POWER_UP_H
#define GT_CLEAN_SLATE_POWER_UP_H

#include "GTPowerUp.h"
#include "../../../GT/GameLogic/Obstacle/GTObstacleSpawner.h"

USING_NS_MK

NS_GT_BEGIN

class GTCleanSlatePowerUp : public GTPowerUp
{
	typedef GTPowerUp Super;

protected:
	static const gtString m_SpriteFileName;
	static const mkString m_OnCollectSoundName;

	MKSprite* m_objectSprite = NULL;
	GTObstacleSpawner* m_ObstacleSpawner;

	virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
	GT_INITIALISECONTACTLISTENER(GTCleanSlatePowerUp);
	GT_DEINITIALISECONTACTLISTENER(GTCleanSlatePowerUp);

	static GTCleanSlatePowerUp* Create(MKScene* _scene, GTObstacleSpawner* obstacleSpawner);

CC_CONSTRUCTOR_ACCESS:
	GTCleanSlatePowerUp(MKScene* _scene, GTObstacleSpawner* _obstacleSpawner) :
		GTPowerUp(_scene),
		m_ObstacleSpawner(_obstacleSpawner)
	{}
	virtual ~GTCleanSlatePowerUp()
	{
		DeinitialiseContactListener();
	}

	virtual bool init();
};

NS_GT_END

#endif // !GT_CLEAN_SLATE_POWER_UP_H