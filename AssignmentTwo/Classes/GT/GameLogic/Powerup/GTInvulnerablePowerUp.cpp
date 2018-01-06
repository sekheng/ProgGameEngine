#include "GTInvulnerablePowerUp.h"

USING_NS_GT

const mkString GTInvulverablePowerUp::InvulnerableSpriteFilePath = "Tileset/coin (1).png";

GTInvulverablePowerUp::GTInvulverablePowerUp(MKScene *_scene)
    : GTPowerUp(_scene)
    , m_PowerUpSprite(nullptr)
{

}

GTInvulverablePowerUp::~GTInvulverablePowerUp()
{
    DeinitialiseContactListener();
}

GTInvulverablePowerUp *GTInvulverablePowerUp::create(MKScene *_scene)
{
    GTInvulverablePowerUp *invulnerablePtr = new (std::nothrow) GTInvulverablePowerUp(_scene);
    if (invulnerablePtr && invulnerablePtr->init())
    {
        invulnerablePtr->autorelease();
        return invulnerablePtr;
    }
    CC_SAFE_DELETE(invulnerablePtr);
    return nullptr;
}

gtBool GTInvulverablePowerUp::init()
{
    if (!GTPowerUp::init())
    {
        return false;
    }
    m_PowerUpSprite = MKSprite::Create(InvulnerableSpriteFilePath, false);
    m_PowerUpSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    this->addChild(m_PowerUpSprite);
    this->setContentSize(m_PowerUpSprite->getContentSize());
    m_PowerUpSprite->setPosition(this->getContentSize() * 0.5f);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    cocos2d::PhysicsBody* physicsBody = PhysicsBody::createBox(m_PowerUpSprite->getContentSize() * 0.8f);
    physicsBody->setDynamic(true);
    physicsBody->setGravityEnable(false);
    physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_POWERUP);
    physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER);
    physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
    this->setPhysicsBody(physicsBody);
    InitialiseContactListener();

    Size visibleSize = Director::getInstance()->getVisibleSize();

    setPosition(visibleSize.width * 0.1f, visibleSize.height * 0.2f);
    //setScale(10);
    return true;
}

gtBool GTInvulverablePowerUp::OnContactBegin(cocos2d::PhysicsContact& _contact)
{
    return false;
}
