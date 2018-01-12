#include "GTSphereShieldPowerUp.h"
#include "GTSphereShield.h"
#include "../../../GT/Audio/GTSimperMusicSys.h"

#include <string>

NS_GT_BEGIN

const mkString GTSphereShield::m_OnCollectSoundName = "";

const mkString GTSphereShield::m_SpriteFileName = "Textures/Gameplay/PowerUp/SphereShieldSprite.png";

GTSphereShield* GTSphereShield::Create(MKScene* _scene, Node* _playerNode)
{
	GTSphereShield* shield = new (std::nothrow) GTSphereShield(_scene, _playerNode);
	if (shield && shield->init())
	{
		shield->autorelease();
		return shield;
	}

	CC_SAFE_DELETE(shield);
	return nullptr;
}

gtBool GTSphereShield::init()
{
	if (!Super::init()) { return false; }

	InitialiseContactListener();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// Create the MKSprite
	m_objectSprite = MKSprite::Create(m_SpriteFileName, true);
	m_objectSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(m_objectSprite);
	this->setContentSize(m_objectSprite->getContentSize());
	m_objectSprite->setPosition(this->getContentSize() * 0.5f);

	gtF32 desiredScale = (visibleSize.height * 0.3f) / this->getContentSize().height;
	this->setScale(desiredScale);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	// Create the PhysicsBody.
	cocos2d::PhysicsBody* physicsBody = PhysicsBody::createCircle(this->getContentSize().height * 0.5f);
	physicsBody->setDynamic(false);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_POWERUP);
	physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_OBSTACLE);
	physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
	this->setPhysicsBody(physicsBody);

	GTFollowNodeAction* followAction = GTFollowNodeAction::Create(0.0f, m_PlayerNode, GTFollowNodeAction::FollowAxis::ALL);
	this->runAction(RepeatForever::create(followAction));

	return true;
}

gtBool GTSphereShield::OnContactBegin(cocos2d::PhysicsContact& _contact)
{
	PhysicsShape* physicsShapeA = _contact.getShapeA();
	PhysicsShape* physicsShapeB = _contact.getShapeB();
	// Ignore this collision if we're not involved.
	if (physicsShapeA->getBody() != _physicsBody &&
		physicsShapeB->getBody() != _physicsBody)
	{
		return false;
	}

	PhysicsBody* otherPhysicsBody = (physicsShapeA->getBody() != getPhysicsBody()) ? physicsShapeA->getBody() : physicsShapeB->getBody();
	// Dafuq? How can we collide with ourselves?
	if (otherPhysicsBody == getPhysicsBody())
	{
		return false;
	}

	if (NS_MK::MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_OBSTACLE, otherPhysicsBody->getCategoryBitmask()) == false)
	{
		return false;
	}

	// We have collided.
	auto obstacle = otherPhysicsBody->getNode();
	if (obstacle->getPhysicsBody() == nullptr)
	{
		return false;
	}
	obstacle->removeComponent(getPhysicsBody());

	DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
	this->removeComponent(getPhysicsBody());

	removeFromParent();

	return true;
}


NS_GT_END