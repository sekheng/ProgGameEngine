// Include Game
#include "GameScene.h"

// Include MK
#include "MK/SceneManagement/MKSceneManager.h"

// Include GT
#include "GT/AnimationHandlerNode.h"
#include "GT/AnimTransAct.h"
#include "GT/SimperMusicSys.h"
#include "GT/Game Logic/CharacterStatNode.h"
#include "GT/ObstacleNode.h"

const static int CHARACTER_GROUND_CONTACT_BITMASK = 0x00000001;
using namespace GinTama;

bool GameScene::initWithPhysics()
{
    if (!Super::initWithPhysics())
    {
        return false;
    }

    // Let's do some physics.
    this->getPhysicsWorld()->setGravity(Vec2(0, -200));
    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    InitialiseBackgrounds();
    InitialiseGround();
    InitialiseInput();

	auto sawObj = ObstacleNode::create(
		"Tileset/saw.png",
		 Vec2(500, 500),
		[&](cocos2d::PhysicsContact &_contact) -> bool
		{
			auto nodeA = _contact.getShapeA()->getBody()->getNode();
			auto nodeB = _contact.getShapeB()->getBody()->getNode();

			//CHECK IF THIS NODE IS TAGGED PLAYER
			if (nodeA->getTag() == 1)
			{
				cocos2d::log("nodeA is Player Physics Object");
			}
			else if (nodeB->getTag() == 1)
			{
				cocos2d::log("nodeB is Player Physics Object");
			}
			return true;
		}
	);
	this->addChild(sawObj);

    MKInputManager::GetInstance()->SetCurrentContext(MK_CONTEXT1);
    scheduleUpdate();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteAnim/assignment_sprite.plist");
    Sprite *charaSpr = Sprite::create();
    charaSpr->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Run (1).png"));
    this->addChild(charaSpr);
    charaSpr->setScale(0.5f);
    m_MainCharaNode = charaSpr;
    AnimationHandlerNode *charAnimHandler = AnimationHandlerNode::create();
    charaSpr->addChild(charAnimHandler);
    charAnimHandler->initWithJSON_tag("SpriteAnim/MainCharaData.json");
    //TODO: Change this hardcoded position
    charaSpr->setPosition(Vec2(150, 250));
    Size charaSize = Size(charaSpr->getContentSize().width * 0.8f, charaSpr->getContentSize().height * 0.8f);
    PhysicsBody *charaPhysics = PhysicsBody::createBox(charaSize);
    charaPhysics->setAngularVelocityLimit(0.f);
    charaSpr->setPhysicsBody(charaPhysics);
    charaPhysics->setDynamic(true);
    charaPhysics->setGravityEnable(true);
    charaPhysics->setMass(1.f);
    CharacterStatNode *charaStat = CharacterStatNode::create(charaPhysics);
    charaStat->scheduleUpdate();
    charaSpr->addChild(charaStat);
    charaStat->setSlideDuration(3.0f);
    charaPhysics->setContactTestBitmask(CHARACTER_GROUND_CONTACT_BITMASK);

    auto phyContactListener = EventListenerPhysicsContact::create();
    phyContactListener->onContactBegin = CC_CALLBACK_1(GameScene::Chara_GroundContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(phyContactListener, this);

    //GinTama::SimperMusicSys::GetInstance()->playSound("testbgm");

	return true;
}

void GameScene::InitialiseGround()
{
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_Ground = MKSprite::create("Environment/Ground.png", true);
    m_Ground->setAnchorPoint(Vec2(0.0f, 0.0f));
    m_Ground->setPosition(visibleOrigin.x, visibleOrigin.y);

    float desiredHeight = visibleSize.height * 0.1f;
    float groundHeight = m_Ground->getContentSize().height;
    float desiredScale = desiredHeight / groundHeight;
    m_Ground->setScale(desiredScale, desiredScale);

    float groundWidth = m_Ground->getContentSize().width;	
    float numRepeat = visibleSize.width / (groundWidth * desiredScale);
    m_Ground->SetRepeat(numRepeat, 1.0f);

    auto physicsBody = PhysicsBody::createBox(Size(m_Ground->getContentSize().width, m_Ground->getContentSize().height));
    physicsBody->setDynamic(false);
    physicsBody->setContactTestBitmask(CHARACTER_GROUND_CONTACT_BITMASK);
    m_Ground->setPhysicsBody(physicsBody);

    this->addChild(m_Ground);
}

void GameScene::InitialiseBackgrounds()
{
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_Backgrounds = new MKSprite*[NUM_BACKGROUNDLAYERS];
	for (unsigned int i = 0; i < NUM_BACKGROUNDLAYERS; ++i) { m_Backgrounds[i] = nullptr; }

	m_Backgrounds[SKY] = MKSprite::create("Environment/Backgrounds/Background_Sky.png", true);
	m_Backgrounds[REAR] = MKSprite::create("Environment/Backgrounds/Background_Rear.png", true);
	m_Backgrounds[MIDDLE] = MKSprite::create("Environment/Backgrounds/Background_Middle.png", true);
	m_Backgrounds[FRONT] = MKSprite::create("Environment/Backgrounds/Background_Front.png", true);

	for (unsigned int i = 0; i < NUM_BACKGROUNDLAYERS; ++i)
	{
		if (m_Backgrounds[i] != nullptr)
		{
			m_Backgrounds[i]->setAnchorPoint(Vec2(0.0f, 0.0f));
			m_Backgrounds[i]->setPosition(visibleOrigin.x, visibleOrigin.y);

			float desiredHeight = visibleSize.height;
			float backgroundHeight = m_Backgrounds[i]->getContentSize().height;
			float desiredScale = desiredHeight / backgroundHeight;
			m_Backgrounds[i]->setScale(desiredScale, desiredScale);
			
			float backgroundWidth = m_Backgrounds[i]->getContentSize().width;
			float numRepeat = visibleSize.width / (backgroundWidth * desiredScale);
			m_Backgrounds[i]->SetRepeat(numRepeat, 1.0f);

			addChild(m_Backgrounds[i]);
		}
	}
}

void GameScene::OnButton(EventCustom * _event)
{
    MKInputButton* buttonEvent = static_cast<MKInputButton*>(_event->getUserData());
    switch (buttonEvent->m_ButtonState)
    {
    case MinamiKotori::MKInputButton::ButtonState::PRESS:
        switch (buttonEvent->m_InputName)
        {
        case MinamiKotori::MKInputName::JUMP:
        {
            CharacterStatNode *charaStat = m_MainCharaNode->getChildByTag<CharacterStatNode*>(1);
            switch (charaStat->getCurrentState())
            {
            case CHARACTER_STATE::RUNNING:
                // then character jump!
                charaStat->setState(CHARACTER_STATE::SLIDE);
                //charaStat->setState(JUMPING);
                //m_physicsNode->applyImpulse(Vec2(0, 200.f));
                //SimperMusicSys::GetInstance()->playSound("Jump");
                break;
            default:
                break;
            }
        }
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void GameScene::OnClick(EventCustom * _event)
{
}

void GameScene::OnAxis(EventCustom * _event)
{
}

void GameScene::ScrollBackgrounds(float _deltaTime)
{
	m_Ground->OffsetTexture(_deltaTime * 3.0f, 0.0f);
	
	m_Backgrounds[REAR]->OffsetTexture(_deltaTime * 0.05f, 0.0f);
	m_Backgrounds[MIDDLE]->OffsetTexture(_deltaTime * 0.075f, 0.0f);
	m_Backgrounds[FRONT]->OffsetTexture(_deltaTime * 0.1, 0.0f);
}

void GameScene::Deinitialise()
{
	DeinitialiseInput();
}

void GameScene::update(float _deltaTime)
{
    ScrollBackgrounds(_deltaTime);
}

bool GameScene::Chara_GroundContactBegin(PhysicsContact &_contact)
{
    if (CompareBitmasks(_contact.getShapeA()->getContactTestBitmask(), _contact.getShapeB()->getContactTestBitmask()))
    {
        auto zeCharaBodyPhy = m_MainCharaNode->getPhysicsBody();
        zeCharaBodyPhy->setVelocity(Vec2(zeCharaBodyPhy->getVelocity().x, 0.f));
        // this means the character touched the ground!
        m_MainCharaNode->getChildByTag<AnimationHandlerNode*>(69)->transitState("Idle");
        m_MainCharaNode->getChildByTag<CharacterStatNode*>(1)->setState(RUNNING);
    }
    return true;
}

bool GameScene::CompareBitmasks(mkU32 _maskA, mkU32 _maskB)
{
    mkU32 largerNum = _maskA;
    if (largerNum < _maskB)
        largerNum = _maskB;
    mkU32 zeComparedMask = (_maskA | _maskB);
    // if the bits still remains the same after that, the values are the same
    if (zeComparedMask == largerNum)
        return true;
    return false;
}