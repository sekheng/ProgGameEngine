#ifndef GT_SPHERE_SHIELD_POWER_UP_H
#define GT_SPHERE_SHIELD_POWER_UP_H

#include "GTPowerUp.h"

USING_NS_MK

NS_GT_BEGIN

class GTSphereShieldPowerUp : public GTPowerUp
{
	typedef GTPowerUp Super;

protected:
	static const gtString m_SpriteFileName;
	static const mkString m_OnCollectSoundName;

	MKSprite* m_objectSprite = NULL;

	virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
	GT_INITIALISECONTACTLISTENER(GTSphereShieldPowerUp);
	GT_DEINITIALISECONTACTLISTENER(GTSphereShieldPowerUp);

	static GTSphereShieldPowerUp* Create(MKScene* _scene);

CC_CONSTRUCTOR_ACCESS:
	GTSphereShieldPowerUp(MKScene* _scene) :
		GTPowerUp(_scene)
	{}
	virtual ~GTSphereShieldPowerUp()
	{
		DeinitialiseContactListener();
	}

	virtual bool init();
};

NS_GT_END

#endif // !GT_SPHERE_SHIELD_POWER_UP_H