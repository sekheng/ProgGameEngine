// Include GT
#include "GTObstacle_Saw.h"
#include "../../../GT/Animation/GTAnimationHandlerNode.h"
#include "../../Actions/GTMoveBySinAction.h"

// Include STL
#include <string>

NS_GT_BEGIN

// Sprite
const mkString GTObstacle_Saw::m_SawSpriteFile = "Textures/Gameplay/Obstacle/Saw/Saw.png";

// Audio
const mkString GTObstacle_Saw::m_SawSpinningSoundName = "Saw_Spinning";
const mkString GTObstacle_Saw::m_SawHitSoundName = "Saw_Hit";

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
	cocos2d::PhysicsBody* physicsBody = PhysicsBody::createCircle(m_Saw->getContentSize().height * 0.5f);
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_OBSTACLE);
	physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER);
	physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
	this->setPhysicsBody(physicsBody);
	InitialiseContactListener();

	gtF32 desiredObstacleScale = (visibleSize.height * 0.2f) / this->getContentSize().height;
	this->setScale(desiredObstacleScale, desiredObstacleScale);

    // Run actions
    auto moveAction = GTMoveBySinAction::Create(8.0f, 0.0f, visibleSize.height * 0.5f);
    auto rotateAction = RotateBy::create(moveAction->getDuration(), m_RotationSpeed * moveAction->getDuration());
    this->runAction(RepeatForever::create(Spawn::createWithTwoActions(moveAction, rotateAction)));

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

	DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
	this->removeComponent(getPhysicsBody());

    // Play Saw Hit Audio
    GTSimperMusicSys::GetInstance()->playSound(m_SawHitSoundName);

	return true;
}

void GTObstacle_Saw::update(gtF32 _deltaTime)
{
    // Play the saw audio when on screen
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //gtF32 screenLeft = GetScene()->getDefaultCamera()->getPositionX() - visibleSize.width * 0.5f;
    gtF32 screenRight = GetScene()->getDefaultCamera()->getPositionX() + visibleSize.width * 0.5f;
    gtF32 obstacleLeft = getPositionX() - (getContentSize().width * 0.5f * getScaleX());

    if (m_SawSpinningSoundID == GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
    {
        if (obstacleLeft < screenRight)
        {
            m_SawSpinningSoundID = GTSimperMusicSys::GetInstance()->playSound(m_SawSpinningSoundName);
        }
    }
}

NS_GT_END