// Include Game
#include "GameScene.h"

// Include MK
#include "..\MK\SceneManagement\MKSceneManager.h"

// Include GT
#include "..\GT\Animation\GTAnimationHandlerNode.h"
#include "..\GT\Animation\GTAnimTransAct.h"
#include "..\GT\Audio\GTSimperMusicSys.h"
#include "..\GT\GameLogic\GTCharacterStatNode.h"
#include "..\GT\GameLogic\Obstacle\GTObstacleNode.h"

using namespace GinTama;

bool GameScene::initWithPhysics()
{
    if (!Super::initWithPhysics())
    {
        return false;
    }

    // Let's do some physics.
    this->getPhysicsWorld()->setGravity(Vec2(0, -2000));
    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    InitialiseBackgrounds();
    InitialiseGround();
    InitialiseInput();

    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_1);
    scheduleUpdate();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteAnim/assignment_sprite.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteAnim/assignment_sprite2.plist");
    Sprite *charaSpr = Sprite::create();
    charaSpr->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Run (1).png"));
    this->addChild(charaSpr);
    charaSpr->setScale(0.5f);
    m_MainCharaNode = charaSpr;
    GTAnimationHandlerNode *charAnimHandler = GTAnimationHandlerNode::create();
    charaSpr->addChild(charAnimHandler);
    charAnimHandler->initWithJSON_tag("SpriteAnim/MainCharaData.json");
    //TODO: Change this hardcoded position
    charaSpr->setPosition(Vec2(150, 250));
    Size charaSize = Size(charaSpr->getContentSize().width * 0.8f, charaSpr->getContentSize().height * 0.8f);
    PhysicsBody *charaPhysics = PhysicsBody::createBox(charaSize);
    charaPhysics->setAngularVelocityLimit(0.f);
    charaPhysics->setMass(10.0f);
    charaSpr->setPhysicsBody(charaPhysics);
    charaPhysics->setDynamic(true);
    charaPhysics->setGravityEnable(true);
    GTCharacterStatNode *charaStat = GTCharacterStatNode::create(this, charaPhysics);
    charaStat->scheduleUpdate();
    charaSpr->addChild(charaStat);
    charaStat->setSlideDuration(1.0f);
    charaStat->setDashDuration(1.0f);
    charaStat->setSpeedX(0.1f);
    //charaPhysics->setCategoryBitmask(GT_COLLISION_CATEGORY_PLAYER);
    //charaPhysics->setCollisionBitmask(GT_COLLISION_CATEGORY_GROUND);
    //charaPhysics->setContactTestBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_OBSTACLE);

    //auto phyContactListener = EventListenerPhysicsContact::create();
    //phyContactListener->onContactBegin = CC_CALLBACK_1(GameScene::Chara_GroundContactBegin, this);
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(phyContactListener, this);


    // Create Obstacle Spawner
    InitialiseObstacles();

	return true;
}

void GameScene::InitialiseGround()
{
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_Ground = MKSprite::Create("Textures/Environment/Ground.png", true);
    m_Ground->setAnchorPoint(Vec2(0.0f, 0.0f));
    m_Ground->setPosition(visibleOrigin.x, visibleOrigin.y);

    // Scale the ground to be the correct size. We want the ground to be 0.1 of the screen height.
    float groundHeight = m_Ground->getContentSize().height;
    float desiredHeight = visibleSize.height * 0.1f;
    float desiredScale = desiredHeight / groundHeight;
    m_Ground->setScale(desiredScale * 1000.0f, desiredScale);
    m_Ground->SetTextureScale(1000.0f, 1.0f);

    auto physicsBody = PhysicsBody::createBox(Size(m_Ground->getContentSize().width, m_Ground->getContentSize().height));
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_GROUND);
    physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_PLAYER);
    physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_PLAYER);
    m_Ground->setPhysicsBody(physicsBody);

    this->addChild(m_Ground);
}

void GameScene::InitialiseBackgrounds()
{
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_Backgrounds = new MKSprite*[NUM_BACKGROUNDLAYERS];
	for (unsigned int i = 0; i < NUM_BACKGROUNDLAYERS; ++i) { m_Backgrounds[i] = nullptr; }

	m_Backgrounds[SKY] = MKSprite::Create("Textures/Backgrounds/Background_Sky.png", true);
	m_Backgrounds[REAR] = MKSprite::Create("Textures/Backgrounds/Background_Rear.png", true);
	m_Backgrounds[MIDDLE] = MKSprite::Create("Textures/Backgrounds/Background_Middle.png", true);
	m_Backgrounds[FRONT] = MKSprite::Create("Textures/Backgrounds/Background_Front.png", true);

	for (unsigned int i = 0; i < NUM_BACKGROUNDLAYERS; ++i)
	{
		if (m_Backgrounds[i] != nullptr)
		{
			m_Backgrounds[i]->setAnchorPoint(Vec2(0.0f, 0.0f));
			m_Backgrounds[i]->setPosition(visibleOrigin.x, visibleOrigin.y);

            // We want the background to fill up the whole screen.
            float backgroundWidth = m_Backgrounds[i]->getContentSize().width;
            float backgroundHeight = m_Backgrounds[i]->getContentSize().height;
            float backgroundAspectRatio = backgroundWidth / backgroundHeight;

            float desiredWidth = visibleSize.width;
            float desiredHeight = visibleSize.height;

			m_Backgrounds[i]->setScale(desiredWidth / backgroundWidth, desiredHeight / backgroundHeight);
            m_Backgrounds[i]->SetTextureScale(backgroundWidth / desiredWidth, 1.0f);

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
    {
        GTCharacterStatNode *charaStat = m_MainCharaNode->getChildByTag<GTCharacterStatNode*>(1);
        switch (buttonEvent->m_InputName)
        {
        case MinamiKotori::MKInputName::JUMP:
        {
            switch (charaStat->getCurrentState())
            {
            case CHARACTER_STATE::RUNNING:
                // then character jump!
                charaStat->setState(JUMPING);
                m_MainCharaNode->getPhysicsBody()->applyImpulse(Vec2(0, 7500.f));
                GTSimperMusicSys::GetInstance()->playSound("Jump");
                break;
            default:
                break;
            }
        }
        break;
        case MKInputName::SLIDE:
            charaStat->setState(CHARACTER_STATE::SLIDE);
            break;
        case MKInputName::SMASH:
            charaStat->setState(CHARACTER_STATE::DASH);
            break;
        default:
            break;
        }
        break;
    }
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
    m_Backgrounds[REAR]->OffsetTexture(_deltaTime * 0.05f, 0.0f);
	m_Backgrounds[MIDDLE]->OffsetTexture(_deltaTime * 0.075f, 0.0f);
	m_Backgrounds[FRONT]->OffsetTexture(_deltaTime * 0.1, 0.0f);
}

void GameScene::Deinitialise()
{
	DeinitialiseInput();
    DeinitialiseObstacles();
}

void GameScene::update(float _deltaTime)
{
    ScrollBackgrounds(_deltaTime);

    m_ObstacleSpawner->Update(_deltaTime);
}

bool GameScene::Chara_GroundContactBegin(PhysicsContact &_contact)
{
    if (CompareBitmasks(_contact.getShapeA()->getContactTestBitmask(), _contact.getShapeB()->getContactTestBitmask()))
    {
        auto zeCharaBodyPhy = m_MainCharaNode->getPhysicsBody();
        zeCharaBodyPhy->setVelocity(Vec2(zeCharaBodyPhy->getVelocity().x, 0.f));
        // this means the character touched the ground!
        m_MainCharaNode->getChildByTag<GTAnimationHandlerNode*>(69)->transitState("Idle");
        m_MainCharaNode->getChildByTag<GTCharacterStatNode*>(1)->setState(RUNNING);
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

void GameScene::InitialiseObstacles()
{
    DeinitialiseObstacles();

    m_ObstacleSpawner = new GTObstacleSpawner(this, m_MainCharaNode);
}

void GameScene::DeinitialiseObstacles()
{
    if (m_ObstacleSpawner != nullptr)
    {
        delete m_ObstacleSpawner;
    }
}