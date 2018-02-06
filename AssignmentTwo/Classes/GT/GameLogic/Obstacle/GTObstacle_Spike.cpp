// Include GT
#include "GTObstacle_Spike.h"
#include "../../../GT/Animation/GTAnimationHandlerNode.h"
#include "../../../GT/Audio/GTSimperMusicSys.h"
#include "../../Actions/GTRemoveFromParentAction.h"

// Include STL
#include <string>

NS_GT_BEGIN

// Sprite
const mkString GTObstacle_Spike::m_SpikeSpriteFile = "Textures/Gameplay/Obstacle/Spikes/SpikesSingle.png";
const mkString GTObstacle_Spike::m_ExplosionPListFile = "Textures/Gameplay/Obstacle/Spikes/Explosion/Explosion.plist";
const mkString GTObstacle_Spike::m_ExplosionJSONFile = "Textures/Gameplay/Obstacle/Spikes/Explosion/Explosion.json";
const mkString GTObstacle_Spike::m_ExplosionSpriteFrameName = "Explosion_0.png";
const mkString GTObstacle_Spike::m_ExplosionTransitState = "None";

// Audio
const mkString GTObstacle_Spike::m_SpikeHitSoundName = "Spike_Hit";
const mkString GTObstacle_Spike::m_SpikeExplosionSoundName = "Spike_Explosion";

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

void GTObstacle_Spike::DestroyObstacle()
{
    // Remove our physics body.
    DeinitialiseContactListener();
    if (getPhysicsBody() != nullptr)
    {
        this->removeComponent(getPhysicsBody());
    }

    // Stop All Actions
    this->stopAllActions();

    // Explode
    Size visibleSize = Director::getInstance()->getVisibleSize();
    gtF32 screenRight = GetScene()->getDefaultCamera()->getPositionX() + visibleSize.width * 0.5f;
    gtF32 obstacleLeft = getPositionX() - (getContentSize().width * 0.5f * getScaleX());

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(m_ExplosionPListFile);

    cocos2d::Sprite* explosionSprite = cocos2d::Sprite::create();
    explosionSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getInstance()->getSpriteFrameByName(m_ExplosionSpriteFrameName));

    GTAnimationHandlerNode* explosionAnimation = GTAnimationHandlerNode::createWithAutoDestroy(explosionSprite);
    explosionAnimation->initWithJSON_tag(m_ExplosionJSONFile);
    explosionAnimation->transitState(m_ExplosionTransitState);

    // Scale the explosion sprite.
    explosionSprite->setScale((visibleSize.height * 0.3f) / explosionSprite->getContentSize().width, (visibleSize.height * 0.3f) / explosionSprite->getContentSize().height);
    // As the missile is scaled, we have to divide the explosion scale by the missile scale,
    // to get rid of the missile's scaling.
    explosionSprite->setScale(explosionSprite->getScaleX() / this->getScaleX(), explosionSprite->getScaleY() / this->getScaleY());
    explosionSprite->setPosition(this->getContentSize() * 0.5f);

    this->addChild(explosionSprite);

    // Play the explosion audio.
    GTSimperMusicSys::GetInstance()->playSound(m_SpikeExplosionSoundName);

    // Run Actions
    this->runAction(
        Sequence::create(
            DelayTime::create(m_DestroyedAnimationDuration),
            GTRemoveFromParentAction::Create(),
            nullptr
        )
    );
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
    physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER | GT_COLLISION_CATEGORY_SHIELD);
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

    // Only check collision with the player and shield.
    if (!NS_MK::MKMathsHelper::CompareBitmasks<mkS32>(getPhysicsBody()->getContactTestBitmask(), otherPhysicsBody->getCategoryBitmask()))
    {
        return false;
    }

	// Stop everything. The only reason we are not deleting instantly is so that
	DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
	this->removeComponent(getPhysicsBody());

    GTSimperMusicSys::GetInstance()->playSound(m_SpikeHitSoundName);

	return true;
}

NS_GT_END