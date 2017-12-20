// Include GT
#include "GTObstacle_Saw.h"
#include "../../../GT/Animation/GTAnimationHandlerNode.h"

// Include STL
#include <string>

NS_GT_BEGIN

// Sprite
const mkString GTObstacle_Saw::m_SawSpriteFile = "Textures/Gameplay/Obstacle/Saw/Saw.png";

GTObstacle_Saw* GTObstacle_Saw::Create(MKScene* _scene)
{
	GTObstacle_Saw* obstacle = new (std::nothrow) GTObstacle_Saw(_scene);
	if (obstacle && obstacle->init())
	{
		obstacle->autorelease();
		return obstacle;
	}

	CC_SAFE_DELETE(obstacle);
	return nullptr;
}

gtBool GTObstacle_Saw::init()
{
	if (!Super::init()) { return false; }

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

	// Create the MKSprite.
	m_Saw = MKSprite::Create(m_SawSpriteFile, true);
	m_Saw->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(m_Saw);
	// Set our Size
	this->setContentSize(m_Saw->getContentSize());
	// Set the sprite to be in the middle of this node.
	m_Saw->setPosition(this->getContentSize() * 0.5f);

	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	// Create the PhysicsBody.
	cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(Size(m_Saw->getContentSize().width, m_Saw->getContentSize().height));
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_OBSTACLE);
	physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER);
	physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
	this->setPhysicsBody(physicsBody);
	InitialiseContactListener();

	gtF32 desiredObstacleScale = (visibleSize.height * 0.15f) / this->getContentSize().height;
	this->setScale(desiredObstacleScale, desiredObstacleScale);

	// Run actions
	gtF32 horizontalVelocity = GetHorizontalVelocity();
	auto moveBy = MoveBy::create(1.0f, Vec2(horizontalVelocity, sin(sinAngle)));
	//auto moveBy = MoveTo::create(1.0f, Vec2(0.0f, 0.0f));
	//auto sineMovement = EaseSineInOut::create(moveBy);
	this->runAction(RepeatForever::create(moveBy));

	sawSpawnPositionY = visibleSize.height * 0.5f;

	return true;
}

gtBool GTObstacle_Saw::OnContactBegin(cocos2d::PhysicsContact& _contact)
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
	// the smoke can finish their animation.
	DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
	this->removeComponent(getPhysicsBody());
	this->removeChild(m_Saw, false);
	m_Saw = NULL;

	return true;
}

gtF32 GTObstacle_Saw::GetHorizontalVelocity()
{
	return -Director::getInstance()->getVisibleSize().height * 0.5f;
}

void GTObstacle_Saw::update(mkF32 _deltaTime)
{
	sinAngle += _deltaTime * MKMathsHelper::TWO_PI * 0.5f;
	this->setPositionY(sawSpawnPositionY + (150.0 * sin(3.0 * sinAngle)));
}

NS_GT_END