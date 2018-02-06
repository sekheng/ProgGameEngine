// Include GT
#include "GTObstacle_Laser.h"
#include "../../Animation/GTAnimationHandlerNode.h"

// Include STL
#include <string>

NS_GT_BEGIN

// Sprite
const mkString GTObstacle_Laser::m_LaserGunSpriteFile = "Textures/Gameplay/Obstacle/Laser/Laser_Gun.png";
const mkString GTObstacle_Laser::m_BeamPListFile = "Textures/Gameplay/Obstacle/Laser/Beam/Beam.plist";
const mkString GTObstacle_Laser::m_BeamJSONFile = "Textures/Gameplay/Obstacle/Laser/Beam/Beam.json";
const mkString GTObstacle_Laser::m_BeamSpriteFrameName = "Beam_0.png";
const mkString GTObstacle_Laser::m_BeamTransitState = "None";
const mkString GTObstacle_Laser::m_ExplosionPListFile = "Textures/Gameplay/Obstacle/Laser/Explosion/Explosion.plist";
const mkString GTObstacle_Laser::m_ExplosionJSONFile = "Textures/Gameplay/Obstacle/Laser/Explosion/Explosion.json";
const mkString GTObstacle_Laser::m_ExplosionSpriteFrameName = "Explosion_0.png";
const mkString GTObstacle_Laser::m_ExplosionTransitState = "None";

// Audio
const mkString GTObstacle_Laser::m_LaserChargingSoundName = "Laser_Charging";
const mkString GTObstacle_Laser::m_LaserShootingSoundName = "Laser_Shooting";
const mkString GTObstacle_Laser::m_LaserExplosionSoundName = "Laser_Explosion";

// Others
const gtF32 GTObstacle_Laser::m_MoveDownDuration = 1.5f;
const gtF32 GTObstacle_Laser::m_LaserBeamChargeDuration = 1.5f;
const gtF32 GTObstacle_Laser::m_LaserBeamShootDuration = 0.2;
const gtF32 GTObstacle_Laser::m_MoveUpDuration = 2.0f;

GTObstacle_Laser* GTObstacle_Laser::Create(MKScene* _scene, gtF32 _spawnDelay)
{
    GTObstacle_Laser* obstacle = new (std::nothrow) GTObstacle_Laser(_scene);
    if (obstacle && obstacle->init(_spawnDelay))
    {
        obstacle->autorelease();
        return obstacle;
    }

    CC_SAFE_DELETE(obstacle);
    return nullptr;
}

void GTObstacle_Laser::DestroyObstacle()
{
    // Remove our physics body.
    DeinitialiseContactListener();
    if (getPhysicsBody() != nullptr)
    {
        this->removeComponent(getPhysicsBody());
    }

    // Stop All Actions
    this->stopAllActions();

    // If we aren't on screen yet, simply remove from parent without any animations.
    if (!m_LaserGunLeft && !m_LaserGunRight)
    {
        Super::DestroyObstacle();
        return;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Create Explosion Sprite Animation (Left Laser Gun)
    {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(m_ExplosionPListFile);
        cocos2d::Sprite* explosionSprite = cocos2d::Sprite::create();
        explosionSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getInstance()->getSpriteFrameByName(m_ExplosionSpriteFrameName));
        GTAnimationHandlerNode* explosionAnimation = GTAnimationHandlerNode::createWithAutoDestroy(explosionSprite);
        explosionAnimation->initWithJSON_tag(m_ExplosionJSONFile);
        explosionAnimation->transitState(m_ExplosionTransitState);
        explosionSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        explosionSprite->setPosition(m_LaserGunLeft->getPosition());
        explosionSprite->setScale((visibleSize.height * 0.3f) / explosionSprite->getContentSize().width, (visibleSize.height * 0.3f) / explosionSprite->getContentSize().height);
        addChild(explosionSprite, m_LaserGunZPriority + 1);
    }

    // Create Explosion Sprite Animation (Right Laser Gun)
    {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(m_ExplosionPListFile);
        cocos2d::Sprite* explosionSprite = cocos2d::Sprite::create();
        explosionSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getInstance()->getSpriteFrameByName(m_ExplosionSpriteFrameName));
        GTAnimationHandlerNode* explosionAnimation = GTAnimationHandlerNode::createWithAutoDestroy(explosionSprite);
        explosionAnimation->initWithJSON_tag(m_ExplosionJSONFile);
        explosionAnimation->transitState(m_ExplosionTransitState);
        explosionSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        explosionSprite->setPosition(m_LaserGunRight->getPosition());
        explosionSprite->setScale((visibleSize.height * 0.3f) / explosionSprite->getContentSize().width, (visibleSize.height * 0.3f) / explosionSprite->getContentSize().height);
        addChild(explosionSprite, m_LaserGunZPriority + 1);
    }

    // Play the explosion audio.
    GTSimperMusicSys::GetInstance()->playSound(m_LaserExplosionSoundName);

    // Run Actions
    auto followAction = GTFollowNodeAction::Create(m_DestroyedAnimationDuration, GetScene()->getDefaultCamera(), GTFollowNodeAction::FollowAxis::X);
    this->runAction(
        Spawn::createWithTwoActions(followAction,
            Sequence::create(
            DelayTime::create(m_DestroyedAnimationDuration),
            GTRemoveFromParentAction::Create(),
            nullptr)
        )
    );
}

gtBool GTObstacle_Laser::init(gtF32 _spawnDelay)
{
    if (!Super::init()) { return false; }

    m_SpawnDelay = _spawnDelay;
    m_TotalDuration = m_MoveDownDuration + m_LaserBeamChargeDuration + m_LaserBeamShootDuration + m_MoveUpDuration + m_SpawnDelay;

    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Set the anchor point.
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    // Other Actions Action
    auto followAction = GTFollowNodeAction::Create(m_TotalDuration, GetScene()->getDefaultCamera(), GTFollowNodeAction::FollowAxis::X);
    auto spawnGunsCallback = CallFunc::create(CC_CALLBACK_0(GTObstacle_Laser::SpawnLaserGuns, this));
    auto spawnBeamCallback = CallFunc::create(CC_CALLBACK_0(GTObstacle_Laser::SpawnLaserBeam, this));

    // Move Actions
    auto moveDownAction = MoveBy::create(m_MoveDownDuration, cocos2d::Vec3(0.0f, -visibleSize.height * 0.7f, 0.0f));
    auto moveUpAction = MoveBy::create(m_MoveUpDuration, cocos2d::Vec3(0.0f, visibleSize.height * 0.7f, 0.0f) * 2.0f);

    // Particles Callbacks
    auto spawnParticlesCallback = CallFunc::create(CC_CALLBACK_0(GTObstacle_Laser::SpawnParticles, this));

    // Physics Body Callbacks
    auto spawnPhysicsBodyCallback = CallFunc::create(CC_CALLBACK_0(GTObstacle_Laser::SpawnPhysicsBody, this));
    auto despawnPhysicsBodyCallback = CallFunc::create(CC_CALLBACK_0(GTObstacle_Laser::DespawnPhysicsBody, this));

    this->runAction(Spawn::createWithTwoActions(followAction,
        Sequence::create(
            DelayTime::create(m_SpawnDelay),
            spawnGunsCallback,
            // Move the laser into position.
            moveDownAction,
            // Charge up the beam.
            spawnParticlesCallback,
            DelayTime::create(m_LaserBeamChargeDuration),
            // Shoot the beam.
            spawnBeamCallback,
            spawnPhysicsBodyCallback,
            DelayTime::create(m_LaserBeamShootDuration),
            // Stop the beam.
            despawnPhysicsBodyCallback,
            // Move the laser away.
            moveUpAction,
            // End of sequence.
            NULL
        )
    ));

    return true;
}

gtBool GTObstacle_Laser::OnContactBegin(cocos2d::PhysicsContact& _contact)
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

    DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
    if (getPhysicsBody() != nullptr)
    {
        this->removeComponent(getPhysicsBody());
    }

    return true;
}

void GTObstacle_Laser::SpawnLaserGuns()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Create the sprites.
    m_LaserGunLeft = MKSprite::Create(m_LaserGunSpriteFile, true);
    m_LaserGunLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(m_LaserGunLeft, m_LaserGunZPriority);
    m_LaserGunLeft->setPosition(-visibleSize.width * 0.5f, 0.5f);

    // Make the right laser gun face the opposite direction.
    m_LaserGunRight = MKSprite::Create(m_LaserGunSpriteFile, true);
    m_LaserGunRight->SetTextureScale(-1.0f, 1.0f);
    m_LaserGunRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    this->addChild(m_LaserGunRight, m_LaserGunZPriority);
    m_LaserGunRight->setPosition(visibleSize.width * 0.5f, 0.5f);

    // Scale the laser guns to the correct size.
    gtF32 desiredLaserGunSize = (visibleSize.height * 0.05f) / m_LaserGunLeft->getContentSize().height;
    m_LaserGunLeft->setScale(desiredLaserGunSize);
    m_LaserGunRight->setScale(desiredLaserGunSize);
}

void GTObstacle_Laser::SpawnParticles()
{
    // Create our particles for the left gun.
    ParticleFire* particlesLeft = ParticleFire::createWithTotalParticles(200);
    particlesLeft->setEmissionRate(particlesLeft->getTotalParticles() / (m_LaserBeamChargeDuration + m_LaserBeamShootDuration));
    particlesLeft->setPositionType(cocos2d::ParticleSystem::PositionType::RELATIVE);
    particlesLeft->setDuration(m_LaserBeamChargeDuration + m_LaserBeamShootDuration);
    particlesLeft->setLife(0.1f);
    particlesLeft->setLifeVar(0.05f);
    particlesLeft->setStartSize(m_LaserGunLeft->getContentSize().height);
    particlesLeft->setStartSizeVar(m_LaserGunLeft->getContentSize().height * 0.1f);
    particlesLeft->setEndSize(m_LaserGunLeft->getContentSize().height * 3.0f);
    particlesLeft->setEndSizeVar(m_LaserGunLeft->getContentSize().height * 0.1f);
    particlesLeft->setAutoRemoveOnFinish(true);
    particlesLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    particlesLeft->setPosition(Vec2(m_LaserGunLeft->getContentSize().width, m_LaserGunLeft->getContentSize().height * 0.5f));
    m_LaserGunLeft->addChild(particlesLeft);

    // Create our particles for the right gun.
    ParticleFire* particlesRight = ParticleFire::createWithTotalParticles(200);
    particlesRight->setEmissionRate(particlesLeft->getTotalParticles() / (m_LaserBeamChargeDuration + m_LaserBeamShootDuration));
    particlesRight->setPositionType(cocos2d::ParticleSystem::PositionType::RELATIVE);
    particlesRight->setDuration(m_LaserBeamChargeDuration + m_LaserBeamShootDuration);
    particlesRight->setLife(0.1f);
    particlesRight->setLifeVar(0.05f);
    particlesRight->setStartSize(m_LaserGunRight->getContentSize().height);
    particlesRight->setStartSizeVar(m_LaserGunRight->getContentSize().height * 0.1f);
    particlesRight->setEndSize(m_LaserGunRight->getContentSize().height);
    particlesRight->setEndSizeVar(m_LaserGunRight->getContentSize().height * 3.0f);
    particlesRight->setAutoRemoveOnFinish(true);
    particlesRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    particlesRight->setPosition(Vec2(0.0f, m_LaserGunRight->getContentSize().height * 0.5f));
    m_LaserGunRight->addChild(particlesRight);

    // Play Audio
    if (m_LaserBeamChargingSoundID == GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
    {
        m_LaserBeamChargingSoundID = GTSimperMusicSys::GetInstance()->playSound(m_LaserChargingSoundName);
    }
}

void GTObstacle_Laser::SpawnLaserBeam()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Create Sprite Animation
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(m_BeamPListFile);
    cocos2d::Sprite* beamSprite = cocos2d::Sprite::create();
    beamSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getInstance()->getSpriteFrameByName(m_BeamSpriteFrameName));
    GTAnimationHandlerNode* beamAnimation = GTAnimationHandlerNode::createWithAutoDestroy(beamSprite);
    beamAnimation->initWithJSON_tag(m_BeamJSONFile);
    beamAnimation->transitState(m_BeamTransitState);
    beamSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    beamSprite->setScale(visibleSize.width / beamSprite->getContentSize().width, (visibleSize.height * 0.05f) / beamSprite->getContentSize().height);

    addChild(beamSprite, m_LaserBeamZPriority);
}

void GTObstacle_Laser::SpawnPhysicsBody()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // Create the PhysicsBody.
    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(Size(visibleSize.width, visibleSize.height * 0.05f));
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_OBSTACLE);
    physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER | GT_COLLISION_CATEGORY_SHIELD);
    physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
    this->setPhysicsBody(physicsBody);
    InitialiseContactListener();

    // Play Audio
    if (m_LaserBeamShootingSoundID == GTSimperMusicSys::SOUND_EFFECT_NOT_FOUND)
    {
        m_LaserBeamShootingSoundID = GTSimperMusicSys::GetInstance()->playSound(m_LaserShootingSoundName);
    }
}

void GTObstacle_Laser::DespawnPhysicsBody()
{
    DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
    if (getPhysicsBody() != nullptr)
    {
        this->removeComponent(getPhysicsBody());
    }
}

NS_GT_END