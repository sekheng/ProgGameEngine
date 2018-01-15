#ifndef GT_SLOW_TIME_POWER_UP_H
#define GT_SLOW_TIME_POWER_UP_H

#include "GTPowerUp.h"

USING_NS_MK

NS_GT_BEGIN

class GTSlowTimePowerUp : public GTPowerUp
{
	typedef GTPowerUp Super;

	

protected:
	static const gtString m_SpriteFileName;
	static const mkString m_OnCollectSoundName;

	MKSprite* m_objectSprite = NULL;

	virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
	static bool m_OnContact;
	float m_countDownTimer = 6.0f;
	static float m_currentCountDownTimer;

	GT_INITIALISECONTACTLISTENER(GTSlowTimePowerUp);
	GT_DEINITIALISECONTACTLISTENER(GTSlowTimePowerUp);

	static GTSlowTimePowerUp* Create(MKScene* _scene);

CC_CONSTRUCTOR_ACCESS:
	GTSlowTimePowerUp(MKScene* _scene) :
		GTPowerUp(_scene)
	{}
	virtual ~GTSlowTimePowerUp()
	{
		DeinitialiseContactListener();
	}

	virtual bool init();
};

NS_GT_END

#endif // !GT_SLOW_TIME_POWER_UP_H



