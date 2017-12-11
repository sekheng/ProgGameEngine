// Include GT
#include "GTObstacle_Missile.h"
#include "GT/Animation/GTAnimationHandlerNode.h"

// Include STL
#include <string>

NS_GT_BEGIN

// Sprite
const mkString GTObstacle_Missile::m_MissileSpriteFile = "Textures/Gameplay/Obstacle/Missile/Missile.png";
const mkString GTObstacle_Missile::m_ExplosionPListFile = "Textures/Gameplay/Obstacle/Missile/Explosion/Explosion.plist";
const mkString GTObstacle_Missile::m_ExplosionJSONFile = "Textures/Gameplay/Obstacle/Missile/Explosion/Explosion.json";
const mkString GTObstacle_Missile::m_ExplosionSpriteFrameName = "Explosion_0.png";

// Audio
const mkString GTObstacle_Missile::m_MissileFlightSoundName = "Missile_Flight";
const mkString GTObstacle_Missile::m_MissileExplosionSoundName = "Missile_Explosion";

GTObstacle_Missile* GTObstacle_Missile::Create(MKScene* _scene)
{
    GTObstacle_Missile* obstacle = new (std::nothrow) GTObstacle_Missile(_scene);
    if (obstacle && obstacle->init())
    {
        obstacle->autorelease();
        return obstacle;
    }

    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

gtBool GTObstacle_Missile::init()
{
    if (!Super::init()) { return false; }

    InitialiseContactListener();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

    // Create the MKSprite.
    m_Missile = MKSprite::Create(m_MissileSpriteFile, false);
    m_Missile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(m_Missile);
	// Set our size.
	this->setContentSize(m_Missile->getContentSize());
	// Set the sprite to be in the middle of this node.
	m_Missile->setPosition(this->getContentSize() * 0.5f);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    // Create the PhysicsBody.
    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(m_Missile->getContentSize() * 0.8f);
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_OBSTACLE);
    physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER);
    physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
    this->setPhysicsBody(physicsBody);

    // Create our particles.
    m_ParticleSmoke = CCParticleSmoke::createWithTotalParticles(400);
    m_ParticleSmoke->setEmissionRate(200.f);
    m_ParticleSmoke->setPositionType(cocos2d::ParticleSystem::PositionType::FREE);
    m_ParticleSmoke->setDuration(cocos2d::ParticleSystem::DURATION_INFINITY);
    m_ParticleSmoke->setLife(0.8f);
    m_ParticleSmoke->setLifeVar(0.2f);
    m_ParticleSmoke->setStartSize(m_Missile->getContentSize().height);
    m_ParticleSmoke->setStartSizeVar(m_Missile->getContentSize().height * 0.1f);
    m_ParticleSmoke->setEndSize(m_Missile->getContentSize().height * 0.1f);
    m_ParticleSmoke->setEndSizeVar(m_Missile->getContentSize().height * 0.1f);
    m_ParticleSmoke->setAutoRemoveOnFinish(false);
    m_ParticleSmoke->setScale(this->getScale());
    m_ParticleSmoke->setPosition(Vec2(this->getContentSize().width, this->getContentSize().height * 0.5f));
    this->addChild(m_ParticleSmoke);

	// Set the missile scale.
	gtF32 desiredObstacleScale = (visibleSize.height * 0.03f) / this->getContentSize().height;
	this->setScale(desiredObstacleScale, desiredObstacleScale);

    // Play the rocket audio.
    m_MissileFlightSoundID = GTSimperMusicSys::GetInstance()->playSound(m_MissileFlightSoundName);

    return true;
}

gtBool GTObstacle_Missile::OnContactBegin(cocos2d::PhysicsContact& _contact)
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

    // Stop everything. The only reason we are not deleting instantly is so that
    // the smoke can finish their animation.
    DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
    if (m_MissileFlightSoundID != GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
    {
        GTSimperMusicSys::GetInstance()->stopSound(m_MissileFlightSoundID);
    }
    GTSimperMusicSys::GetInstance()->playSound(m_MissileExplosionSoundName);
    m_ParticleSmoke->pauseEmissions();
    this->removeComponent(getPhysicsBody());
    this->removeChild(m_Missile, false);
    m_Missile = NULL;

    // Explode
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(m_ExplosionPListFile);

    cocos2d::Sprite* explosionSprite = cocos2d::Sprite::create();
    explosionSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getInstance()->getSpriteFrameByName(m_ExplosionSpriteFrameName));

    GTAnimationHandlerNode* _explosionAnimation = GTAnimationHandlerNode::createWithAutoDestroy(explosionSprite);
    _explosionAnimation->initWithJSON_tag(m_ExplosionJSONFile);
    _explosionAnimation->transitState("None");

    explosionSprite->setPosition(this->getPosition());
    GetScene()->addChild(explosionSprite);

    return false;
}

NS_GT_END