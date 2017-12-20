#ifndef GT_OBSTACLE_SAW_H
#define GT_OBSTACLE_SAW_H

// Include GT
#include "GTObstacleNode.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacle_Saw : public GTObstacleNode
{
	typedef GTObstacleNode Super;

private:
	MKSprite* m_Saw = NULL;
	gtU32 numberOfTicks = 0;
	gtF32 sinAngle = 0;
	gtF32 sawSpawnPositionY;

protected:
	virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
	// Sprite
	static const mkString m_SawSpriteFile;

	// Others
	static const gtF32 m_WarningDuration;

	static GTObstacle_Saw* Create(MKScene* _scene);
	static gtF32 GetHorizontalVelocity();
	static MKSprite* CreateSawWarning();

	GT_INITIALISECONTACTLISTENER(GTObstacle_Saw);
	GT_DEINITIALISECONTACTLISTENER(GTObstacle_Saw);

CC_CONSTRUCTOR_ACCESS:
	// Constructor(s) & Destructor
	GTObstacle_Saw(MKScene* _scene) : GTObstacleNode(_scene) {}
	virtual ~GTObstacle_Saw()
	{
		DeinitialiseContactListener();
	}

	virtual bool init() override;
	virtual void update(mkF32 _deltaTime) override;

};

NS_GT_END

#endif // GT_OBSTACLE_MISSILE_H
