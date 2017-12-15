// Include GT
#include "GTObstacle_Spike.h"
#include "../../../GT/Animation/GTAnimationHandlerNode.h"

// Include STL
#include <string>

NS_GT_BEGIN

// Sprite
const mkString GTObstacle_Spike::m_SpikeSpriteFile = "Textures/Gameplay/Obstacle/Spikes/SpikesSingle.png";

GTObstacle_Spike* GTObstacle_Spike::Create(MKScene* _scene, gtU32 _numberOfSpikes)
{
	GTObstacle_Spike* obstacle = new (std::nothrow) GTObstacle_Spike(_scene, _numberOfSpikes);
	if (obstacle && obstacle->init())
	{
		obstacle->autorelease();
		return obstacle;
	}

	CC_SAFE_DELETE(obstacle);
	return nullptr;
}

gtBool GTObstacle_Spike::init()
{
	if (!Super::init()) { return false; }

	InitialiseContactListener();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

    // Create the MKSprite.
    m_Spike = MKSprite::Create(m_SpikeSpriteFile, true);
    m_Spike->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    m_Spike->SetTextureScale((gtF32)m_NumberOfSpikes, 1.0f);
    this->addChild(m_Spike);
    this->setContentSize(m_Spike->getContentSize());
    // Set the sprite to be in the middle of this node.
    m_Spike->setPosition(this->getContentSize() * 0.5f);
    gtF32 desiredObstacleScale = (visibleSize.height * 0.15f) / this->getContentSize().height;
    this->setScale(desiredObstacleScale * (gtF32)m_NumberOfSpikes, desiredObstacleScale);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

    // Create the PhysicsBody.
    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(Size(m_Spike->getContentSize().width, m_Spike->getContentSize().height));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_OBSTACLE);
    physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER);
    physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
    this->setPhysicsBody(physicsBody);

	return true;
}

gtBool GTObstacle_Spike::OnContactBegin(cocos2d::PhysicsContact& _contact)
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

	return true;
}

NS_GT_END