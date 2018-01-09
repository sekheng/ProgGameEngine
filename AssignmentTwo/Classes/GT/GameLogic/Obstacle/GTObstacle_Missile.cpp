// Include GT
#include "GTObstacle_Missile.h"
#include "../../Animation/GTAnimationHandlerNode.h"

// Include STL
#include <string>

NS_GT_BEGIN

// Sprite
const mkString GTObstacle_Missile::m_MissileSpriteFile = "Textures/Gameplay/Obstacle/Missile/Missile.png";
const mkString GTObstacle_Missile::m_ExplosionPListFile = "Textures/Gameplay/Obstacle/Missile/Explosion/Explosion.plist";
const mkString GTObstacle_Missile::m_ExplosionJSONFile = "Textures/Gameplay/Obstacle/Missile/Explosion/Explosion.json";
const mkString GTObstacle_Missile::m_ExplosionSpriteFrameName = "Explosion_0.png";
const mkString GTObstacle_Missile::m_ExplosionTransitState = "None";
const mkString GTObstacle_Missile::m_MissileWarningSpriteFile = "Textures/Gameplay/Obstacle/Missile/Missile_Warning.png";

// Audio
const mkString GTObstacle_Missile::m_MissileFlightSoundName = "Missile_Flight";
const mkString GTObstacle_Missile::m_MissileExplosionSoundName = "Missile_Explosion";
const mkString GTObstacle_Missile::m_MissileWarningSoundName = "Missile_Warning";

// Others
const gtF32 GTObstacle_Missile::m_WarningDuration = 2.0f;

GTObstacle_Missile* GTObstacle_Missile::Create(MKScene* _scene, mkF32 _playerVelocityX)
{
    GTObstacle_Missile* obstacle = new (std::nothrow) GTObstacle_Missile(_scene);
    if (obstacle && obstacle->init(_playerVelocityX))
    {
        obstacle->autorelease();
        return obstacle;
    }

    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

gtBool GTObstacle_Missile::init(mkF32 _playerVelocityX)
{
    if (!Super::init()) { return false; }

    m_PlayerVelocityX = _playerVelocityX;

	Size visibleSize = Director::getInstance()->getVisibleSize();

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
    InitialiseContactListener();

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

    // Run actions
    gtF32 horizontalVelocity = GetHorizontalVelocity();
    auto moveAction = MoveBy::create(1.0f, Vec2(horizontalVelocity, 0.0f));
    this->runAction(RepeatForever::create(moveAction));

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

    // Only check collision with the player.
    if (!NS_MK::MKMathsHelper::ContainsBitmask<mkS32>(GT_COLLISION_CATEGORY_PLAYER, otherPhysicsBody->getCategoryBitmask()))
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

    GTAnimationHandlerNode* explosionAnimation = GTAnimationHandlerNode::createWithAutoDestroy(explosionSprite);
    explosionAnimation->initWithJSON_tag(m_ExplosionJSONFile);
    explosionAnimation->transitState(m_ExplosionTransitState);

    explosionSprite->setPosition(this->getPosition());
    GetScene()->addChild(explosionSprite);

    return true;
}

gtF32 GTObstacle_Missile::GetHorizontalVelocity()
{
    return -Director::getInstance()->getVisibleSize().height * 2.0f;
}

MKSprite* GTObstacle_Missile::CreateMissileWarning()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    MKSprite* warning = MKSprite::Create(m_MissileWarningSpriteFile, false);
    gtF32 desiredScale = (visibleSize.height * 0.05f) / warning->getContentSize().height;
    warning->setScale(desiredScale);

    GTScaleUpAndDownAction* scaleUpAndDownAction = GTScaleUpAndDownAction::Create(m_WarningDuration, 0.8f * desiredScale, 1.2f * desiredScale, 0.0f, 10.0f);
    GTRemoveFromParentAction* removeFromParentAction = GTRemoveFromParentAction::Create();
    GTPlaySoundAction* playSoundAction = GTPlaySoundAction::Create(m_MissileWarningSoundName);
    warning->runAction(Sequence::create(scaleUpAndDownAction, playSoundAction, removeFromParentAction, NULL));

    return warning;
}

void GTObstacle_Missile::DestroyObstacle()
{
}

void GTObstacle_Missile::update(gtF32 _deltaTime)
{
    // Play the rocket audio when on screen
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //gtF32 screenLeft = GetScene()->getDefaultCamera()->getPositionX() - visibleSize.width * 0.5f;
    gtF32 screenRight = GetScene()->getDefaultCamera()->getPositionX() + visibleSize.width * 0.5f;
    gtF32 obstacleLeft = getPositionX() - (getContentSize().width * 0.5f * getScaleX());

    if (m_MissileFlightSoundID == GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
    {
        if (obstacleLeft < screenRight)
        {
            m_MissileFlightSoundID = GTSimperMusicSys::GetInstance()->playSound(m_MissileFlightSoundName);
        }
    }

    if (m_SpawnedWarning == false)
    {
        mkF32 combinedVelocity = m_PlayerVelocityX - GetHorizontalVelocity();
        if (obstacleLeft <= (screenRight + m_WarningDuration * combinedVelocity))
        {
            MKSprite* warning = CreateMissileWarning();
            warning->setPosition(Vec2(visibleSize.width * 0.95f, this->getPositionY()));
            GetScene()->GetUINode()->addChild(warning);
            m_SpawnedWarning = true;
        }
    }
}

NS_GT_END