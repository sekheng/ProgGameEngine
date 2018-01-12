#ifndef GT_SPHERE_SHIELD_H
#define GT_SPHERE_SHIELD__H

#include "GTPowerUp.h"
#include "../../../GT/GameLogic/GTCharacterStatNode.h"
#include "../../../GT/Actions/GTFollowNodeAction.h"

USING_NS_MK

NS_GT_BEGIN

class GTSphereShield : public GTPowerUp
{
	typedef GTPowerUp Super;

protected:
	static const gtString m_SpriteFileName;
	static const mkString m_OnCollectSoundName;

	MKSprite* m_objectSprite = NULL;
	Node* m_PlayerNode;

	virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact);

public:
	GT_INITIALISECONTACTLISTENER(GTSphereShield);
	GT_DEINITIALISECONTACTLISTENER(GTSphereShield);

	static GTSphereShield* Create(MKScene* _scene, Node* _playerNode);

CC_CONSTRUCTOR_ACCESS:
	GTSphereShield(MKScene* _scene, Node* _playerNode) :
		GTPowerUp(_scene),
		m_PlayerNode(_playerNode)
	{}
	virtual ~GTSphereShield()
	{
		DeinitialiseContactListener();
	}

	virtual bool init();
};

NS_GT_END

#endif // !GT_SPHERE_SHIELD_POWER_UP_H
