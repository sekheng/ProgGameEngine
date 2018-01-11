#include "GTSphereShieldPowerUp.h"
#include "GTSphereShield.h"
#include "../../../GT/Audio/GTSimperMusicSys.h"

#include <string>

NS_GT_BEGIN

const mkString GTSphereShieldPowerUp::m_OnCollectSoundName = "";

const mkString GTSphereShieldPowerUp::m_SpriteFileName = "Textures/Gameplay/PowerUp/SphereShieldSprite.png";

bool GTSphereShieldPowerUp::m_OnContact = false;

GTSphereShieldPowerUp* GTSphereShieldPowerUp::Create(MKScene* _scene, GTCharacterStatNode* _playerNode)
{
	GTSphereShieldPowerUp* powerUp = new (std::nothrow) GTSphereShieldPowerUp(_scene, _playerNode);
	if (powerUp && powerUp->init())
	{
		powerUp->autorelease();
		return powerUp;
	}

	CC_SAFE_DELETE(powerUp);
	return nullptr;
}

gtBool GTSphereShieldPowerUp::init()
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

	cocos2d::PhysicsBody* physicsBody = PhysicsBody::createCircle(m_objectSprite->getContentSize().height * 0.5f);
	physicsBody->setDynamic(true);
	physicsBody->setGravityEnable(false);
	physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_POWERUP);
	physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER);
	physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
	this->setPhysicsBody(physicsBody);
	InitialiseContactListener();

	return true;
}

gtBool GTSphereShieldPowerUp::OnContactBegin(cocos2d::PhysicsContact& _contact)
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
	if (!m_OnContact)
	{
		m_OnContact = true;
	}

	if (m_OnContact)
	{
		GTSphereShield::m_powerUpActivated = true;
		m_OnContact = false;
	}
	if (GTSphereShield::m_powerUpActivated)
	{
		GTSphereShield* shield = GTSphereShield::Create(GetScene(), m_PlayerNode);
		GetScene()->addChild(shield);
	}

	DeinitialiseContactListener(); // Stop listening or else this still gets called somehow.
	this->removeComponent(getPhysicsBody());

	GTSimperMusicSys::GetInstance()->playSound(m_OnCollectSoundName);

	return true;
}

NS_GT_END