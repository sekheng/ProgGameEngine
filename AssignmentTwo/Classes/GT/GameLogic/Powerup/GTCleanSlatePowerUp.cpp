#include "GTCleanSlatePowerUp.h"
#include "../../../GT/Audio/GTSimperMusicSys.h"

#include <string>

NS_GT_BEGIN

const mkString GTCleanSlatePowerUp::m_OnCollectSoundName = "";

const mkString GTCleanSlatePowerUp::m_SpriteFileName = "Textures/Gameplay/PowerUp/CleanSlateSprite.png";

GTCleanSlatePowerUp* GTCleanSlatePowerUp::Create(MKScene* _scene, GTObstacleSpawner* _obstacleSpawner)
{
	GTCleanSlatePowerUp* powerUp = new (std::nothrow) GTCleanSlatePowerUp(_scene, _obstacleSpawner);
	if (powerUp && powerUp->init())
	{
		powerUp->autorelease();
		return powerUp;
	}

	CC_SAFE_DELETE(powerUp);
	return nullptr;
}

gtBool GTCleanSlatePowerUp::init()
{
	if (!Super::init()) { return false; }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

	m_objectSprite = MKSprite::Create(m_SpriteFileName, true);
	m_objectSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(m_objectSprite);
	this->setContentSize(m_objectSprite->getContentSize());
	this->setScale(0.75f);
	m_objectSprite->setPosition(this->getContentSize() * 0.5f);

	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(Size(m_objectSprite->getContentSize().width, m_objectSprite->getContentSize().height));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_POWERUP);
	physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER);
	physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
	this->setPhysicsBody(physicsBody);
	InitialiseContactListener();

	return true;
}

gtBool GTCleanSlatePowerUp::OnContactBegin(cocos2d::PhysicsContact& _contact)
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

	// Only check collision with the player.
	if (!NS_MK::MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_PLAYER, otherPhysicsBody->getCategoryBitmask()))
	{
		return false;
	}

	// Stop everything. The only reason we are not deleting instantly is so that
	DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
	this->removeComponent(getPhysicsBody());

	GTSimperMusicSys::GetInstance()->playSound(m_OnCollectSoundName);

	m_ObstacleSpawner->DestroyObstacles(5);

	return true;
}

NS_GT_END