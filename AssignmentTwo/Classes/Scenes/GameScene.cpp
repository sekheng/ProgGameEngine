// Include Game
#include "GameScene.h"

// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"

// Include GT
#include "../GT/Animation/GTAnimationHandlerNode.h"
#include "../GT/Animation/GTAnimTransAct.h"
#include "../GT/Audio/GTSimperMusicSys.h"
#include "../GT/GameLogic/GTCharacterStatNode.h"
#include "../GT/GameLogic/Obstacle/GTObstacleNode.h"
#include "../GT/GameLogic/Powerup/GTInvulnerablePowerUp.h"

// Include UI
#include "../UIClass/UICreator.h"

using namespace GinTama;

// Overrides
bool GameScene::initWithPhysics()
{
    if (!Super::initWithPhysics())
    {
        return false;
    }

    // Let's do some physics.
    this->getPhysicsWorld()->setGravity(Vec2(0, -3000));
    this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    InitialiseBackgrounds();
    InitialiseGround();
    InitialiseInput();
	InitialiseUI();
    InitialiseText();

    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_1);
    scheduleUpdate();

    InitialisePlayer();

    // Create Obstacle Spawner
    InitialiseObstacles();

    GTInvulverablePowerUp::create(this);

	return true;
}

void GameScene::update(float _deltaTime)
{
    ScrollBackgrounds(_deltaTime);
    UpdateCamera();
    m_ObstacleSpawner->Update(_deltaTime); // This must be updated AFTER the camera.
    UpdateUINode();
    UpdateText();

    // TODO: change this when we have time
    if (m_CharaStatNode->getCurrentState() == DEAD)
    {
        // create all of the UI needed!
        InitialiseGameOverUI();
    }
}

void GameScene::onEnter()
{
    Super::onEnter();

    if (m_ObstacleSpawner) { m_ObstacleSpawner->ResumeAllObstacles(); }
}

void GameScene::onExit()
{
    Super::onExit();

    if (m_ObstacleSpawner) { m_ObstacleSpawner->PauseAllObstacles(); }
}

// Initialisation
void GameScene::InitialisePlayer()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteAnim/assignment_sprite.plist");
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("SpriteAnim/assignment_sprite2.plist");

    // Create the player sprite.
    Sprite *playerSprite = Sprite::create();
    playerSprite->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("Run (1).png"));
    playerSprite->setScale(0.5f);
    addChild(playerSprite);

    // Create player animation.
    GTAnimationHandlerNode* playerAnimationHandler = GTAnimationHandlerNode::create();
    //playerAnimationHandler->initWithJSON_tag("SpriteAnim/MainCharaData.json");
    FileUtils::getInstance()->addSearchPath("SpriteAnim");
    playerAnimationHandler->initWithJSON_tag("SpriteAnim/MainCharaData.json");
    playerSprite->addChild(playerAnimationHandler);

    // Create player physics.
    PhysicsBody* playerPhysicsBody = PhysicsBody::createBox(playerSprite->getContentSize() * 0.8f);
    playerPhysicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_PLAYER);
    playerPhysicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_GROUND);
    playerPhysicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_OBSTACLE);
    playerPhysicsBody->setAngularVelocityLimit(0.0f);
    playerPhysicsBody->setMass(20.0f);
    playerPhysicsBody->setDynamic(true);
    playerPhysicsBody->setGravityEnable(true);
    playerSprite->setPhysicsBody(playerPhysicsBody);

    // Create player stats.
    m_CharaStatNode = GTCharacterStatNode::create(this, playerPhysicsBody);
    m_CharaStatNode->scheduleUpdate();
    m_CharaStatNode->setSlideDuration(0.5f);
    m_CharaStatNode->setSpeedX(visibleSize.height * 0.5f);
    playerSprite->addChild(m_CharaStatNode);

    playerSprite->setPosition(Vec2(0.0f, visibleSize.height * 0.1f + playerSprite->getContentSize().height * 0.5f));
    m_PlayerNode = playerSprite;
}

void GameScene::InitialiseGround()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_Ground = MKSprite::Create("Textures/Environment/Ground.png", true);
    m_Ground->setAnchorPoint(Vec2(0.5f, 0.0f));
    m_Ground->setPosition(Vec2::ZERO);

    // Scale the ground to be the correct size. We want the ground to be 0.1 of the screen height.
    float groundHeight = m_Ground->getContentSize().height;
    float desiredHeight = visibleSize.height * 0.1f;
    float desiredScale = desiredHeight / groundHeight;
    m_Ground->setScale(desiredScale * 2000.0f, desiredScale);
    m_Ground->SetTextureScale(m_Ground->getScaleX() / m_Ground->getScaleY(), 1.0f);

    auto physicsBody = PhysicsBody::createBox(Size(m_Ground->getContentSize().width, m_Ground->getContentSize().height));
    physicsBody->setDynamic(false);
    physicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_GROUND);
    physicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_NONE);
    physicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_NONE);
    m_Ground->setPhysicsBody(physicsBody);

    this->addChild(m_Ground);
}

void GameScene::InitialiseBackgrounds()
{
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
			m_Backgrounds[i]->setAnchorPoint(Vec2::ZERO);
            m_Backgrounds[i]->setPosition(Vec2::ZERO);

            // We want the background to fill up the whole screen.
            float backgroundWidth = m_Backgrounds[i]->getContentSize().width;
            float backgroundHeight = m_Backgrounds[i]->getContentSize().height;
            float backgroundAspectRatio = backgroundWidth / backgroundHeight;

            float desiredWidth = visibleSize.width;
            float desiredHeight = visibleSize.height;

			m_Backgrounds[i]->setScale(desiredWidth / backgroundWidth, desiredHeight / backgroundHeight);
            m_Backgrounds[i]->SetTextureScale(backgroundWidth / desiredWidth, 1.0f);

			GetUINode()->addChild(m_Backgrounds[i]);
		}
	}
}

void GameScene::InitialiseUI()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Sprite* pauseButtonSprite = Sprite::create("PauseIcon.png");

    auto PauseButton = MKUICreator::GetInstance()->createButton(
        Vec2(pauseButtonSprite->getContentSize().width, visibleSize.height - pauseButtonSprite->getContentSize().width),
        "PauseIcon.png",
        "PauseIcon.png",
        "",
        [&](Ref*) -> void
    {
        MKSceneManager::GetInstance()->PushScene("PauseScene");
    }
    );
    GetUINode()->addChild(PauseButton);
}

void GameScene::InitialiseText()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    gtF32 desiredObstacleScale = (visibleSize.height * 24.0f) / this->getContentSize().height;
    m_HighScoreTxt = Label::createWithTTF("HighScore", "fonts/Marker_Felt.ttf", desiredObstacleScale);
    m_HighScoreTxt->setTextColor(Color4B::BLACK);
    m_HighScoreTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    m_HighScoreTxt->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.9f);
    GetUINode()->addChild(m_HighScoreTxt);
}

void GameScene::InitialiseGameOverUI()
{
    // need to ensure that the array of GameOverUI is empty!
    if (m_ArrayOfGameOverUI.size() == 0)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        float UIButtonPosX = (visibleSize.width * 0.5f);
        float UIButtonPosY = (visibleSize.height * 0.5f);
        //RESUME BUTTON//
        auto RetryButton = MKUICreator::GetInstance()->createButton(
            Vec2(UIButtonPosX, UIButtonPosY),
            "ButtonNormal.png",
            "ButtonSelected.png",
            "Retry",
            [&](Ref*) -> void
        {
            //MKSceneManager::GetInstance()->ReplaceScene("GameScene");
            // cannot replace the current scene in the same scene
            MKSceneManager::GetInstance()->ReplaceScene("GameOverScene");
        }
        );
        GetUINode()->addChild(RetryButton);
        m_ArrayOfGameOverUI.push_back(RetryButton);
        //SETTINGS BUTTON//
        auto ReviveButton = MKUICreator::GetInstance()->createButton(
            Vec2(UIButtonPosX, UIButtonPosY - RetryButton->getContentSize().height),
            "ButtonNormal.png",
            "ButtonSelected.png",
            "Revive: " + std::to_string(m_CharaStatNode->getReviveCounter()),
            [&](Ref*) -> void
        {
            if (m_CharaStatNode->getReviveCounter() > 0)
            {
                // have to clear the buttons
                ClearGameOverUI();
                // then revive the player!
                m_CharaStatNode->setState(REVIVE);
            }
        }
        );
        GetUINode()->addChild(ReviveButton);
        m_ArrayOfGameOverUI.push_back(ReviveButton);
        //MAIN MENU BUTTON//
        auto ToMainMenuButton = MKUICreator::GetInstance()->createButton(
            Vec2(UIButtonPosX, UIButtonPosY - (RetryButton->getContentSize().height * 2)),
            "ButtonNormal.png",
            "ButtonSelected.png",
            "Main Menu",
            [&](Ref*) -> void
        {
            MKSceneManager::GetInstance()->ReplaceScene("MainMenuScene");
        }
        );
        GetUINode()->addChild(ToMainMenuButton);
        m_ArrayOfGameOverUI.push_back(ToMainMenuButton);
    }
}

void GameScene::InitialiseObstacles()
{
    DeinitialiseObstacles();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    m_ObstacleSpawner = new GTObstacleSpawner(this, m_PlayerNode, m_CharaStatNode->getSpeedX(), visibleSize.width * 2.0f);
}

// Update
void GameScene::ScrollBackgrounds(float _deltaTime)
{
    m_Backgrounds[REAR]->OffsetTexture(_deltaTime * 0.05f, 0.0f);
	m_Backgrounds[MIDDLE]->OffsetTexture(_deltaTime * 0.075f, 0.0f);
	m_Backgrounds[FRONT]->OffsetTexture(_deltaTime * 0.1, 0.0f);
}

void GameScene::UpdateCamera()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    getDefaultCamera()->setPosition(Vec2(m_PlayerNode->getPositionX() + visibleSize.width * 0.3f, visibleSize.height * 0.5f));
}

void GameScene::UpdateUINode()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_UINode->setPosition(getDefaultCamera()->getPositionX() - visibleSize.width * 0.5f, getDefaultCamera()->getPositionY() - visibleSize.height * 0.5f);
}

void GameScene::UpdateText()
{
    std::string highScoreString = "HighScore: " + std::to_string((mkS32)getDefaultCamera()->getPositionX());
    m_HighScoreTxt->setString(highScoreString);
}

// Deinitialisation
void GameScene::Deinitialise()
{
	DeinitialiseInput();
    DeinitialiseObstacles();
    ClearGameOverUI();
}

void GameScene::DeinitialiseObstacles()
{
    if (m_ObstacleSpawner != nullptr)
    {
        delete m_ObstacleSpawner;
    }
}

void GameScene::ClearGameOverUI()
{
    // clear the Vector of UI
    for (std::vector<Node*>::iterator it = m_ArrayOfGameOverUI.begin(), end = m_ArrayOfGameOverUI.end(); it != end; ++it)
    {
        GetUINode()->removeChild(*it);
    }
    m_ArrayOfGameOverUI.clear();
}

// Input Callbacks
void GameScene::OnButton(EventCustom * _event)
{
    MKInputButton* buttonEvent = static_cast<MKInputButton*>(_event->getUserData());
    switch (buttonEvent->m_ButtonState)
    {
    case MinamiKotori::MKInputButton::ButtonState::PRESS:
    {
        switch (buttonEvent->m_InputName)
        {
        case MinamiKotori::MKInputName::JUMP:
        {
            m_CharaStatNode->CharJump();
        }
        break;
        case MKInputName::SLIDE:
            m_CharaStatNode->setState(CHARACTER_STATE::SLIDE);
            break;
        case MKInputName::SMASH:
            //charaStat->setState(CHARACTER_STATE::DASH);
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