#ifndef GT_SPHERE_SHIELD_H
#define GT_SPHERE_SHIELD__H

#include "GTPowerUp.h"
#include "../../../GT/GameLogic/GTCharacterStatNode.h"

USING_NS_MK

NS_GT_BEGIN

class GTSphereShield : public GTPowerUp
{
	typedef GTPowerUp Super;

protected:
	static const gtString m_SpriteFileName;
	static const mkString m_OnCollectSoundName;

	MKSprite* m_objectSprite = NULL;
	GTCharacterStatNode* m_PlayerNode;

	virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
	static bool m_powerUpActivated;

	GT_INITIALISECONTACTLISTENER(GTSphereShield);
	GT_DEINITIALISECONTACTLISTENER(GTSphereShield);

	static GTSphereShield* Create(MKScene* _scene, GTCharacterStatNode* _playerNode);

CC_CONSTRUCTOR_ACCESS:
	GTSphereShield(MKScene* _scene, GTCharacterStatNode* _playerNode) :
		GTPowerUp(_scene),
		m_PlayerNode(_playerNode)
	{}
	virtual ~GTSphereShield()
	{
		DeinitialiseContactListener();
	}

	virtual bool init();
	virtual void update(gtF32 _deltaTime);
};

NS_GT_END

#endif // !GT_SPHERE_SHIELD_POWER_UP_H
