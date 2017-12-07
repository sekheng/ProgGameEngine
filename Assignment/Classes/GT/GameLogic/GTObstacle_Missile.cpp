// Include GT
#include "GTObstacle_Missile.h"

NS_GT_BEGIN

bool GTObstacle_Missile::init()
{
    if (Super::init() == false)
    {
        return false;
    }

    // Create the MKSprite.
    MKSprite* rocketSprite = MKSprite::create("Sprites/Obstacles/Rocket.png", false);

    // Create the PhysicsBody.
    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(Size(rocketSprite->getContentSize().width * 0.8f, rocketSprite->getContentSize().height * 0.8f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_OBSTACLE);
    physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER);
    physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);

    // Add them to this node.
    this->setPhysicsBody(physicsBody);
    this->addChild(rocketSprite);

    return true;
}

void GTObstacle_Missile::update(float _deltaTime)
{
    Super::update(_deltaTime);

    // Move the missile.
    this->_position += m_Speed * _deltaTime;

    // Using particles, create a trail.
}

bool GTObstacle_Missile::OnContactBegin(cocos2d::PhysicsContact& _contact)
{
    // Explode

    return false;
}

NS_GT_END