#include "GTSphereShieldPowerUp.h"
#include "GTSphereShield.h"
#include "../../../GT/Audio/GTSimperMusicSys.h"

#include <string>

NS_GT_BEGIN

const mkString GTSphereShield::m_OnCollectSoundName = "";

const mkString GTSphereShield::m_SpriteFileName = "Textures/Gameplay/PowerUp/SphereShieldSprite.png";

bool GTSphereShield::m_powerUpActivated = false;

GTSphereShield* GTSphereShield::Create(MKScene* _scene, GTCharacterStatNode* _playerNode)
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

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

	m_objectSprite = MKSprite::Create(m_SpriteFileName, true);
	m_objectSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(m_objectSprite);
	this->setContentSize(m_objectSprite->getContentSize());
	this->setScale(1.5f);
	m_objectSprite->setPosition(this->getContentSize() * 0.5f);

	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	cocos2d::PhysicsBody* physicsBody = PhysicsBody::createCircle(m_objectSprite->getContentSize().height * 0.5f);
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_POWERUP);
	physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_OBSTACLE);
	physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
	this->setPhysicsBody(physicsBody);
	InitialiseContactListener();

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

	if (NS_MK::MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_PLAYER, otherPhysicsBody->getCategoryBitmask()))
	{
		return false;
	}

	// Only check collision with the player.
	if (!NS_MK::MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_OBSTACLE, otherPhysicsBody->getCategoryBitmask()))
	{
		return false;
	}

	if (GTSphereShield::m_powerUpActivated)
	{
		setPosition(m_PlayerNode->getPosition());
	}
	if (NS_MK::MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_OBSTACLE, otherPhysicsBody->getCategoryBitmask()))
	{
		auto obstacle = otherPhysicsBody->getNode();
		if (obstacle->getPhysicsBody() == nullptr)
			return false;
		else
			obstacle->removeComponent(getPhysicsBody());
	}

	DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
	this->removeComponent(getPhysicsBody());

	removeFromParent();

	return true;
}

void GTSphereShield::update(gtF32 _deltaTime)
{
	setPosition(m_PlayerNode->getPosition());
}

NS_GT_END