// Include Game
#include "GameScene.h"

// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"
#include "../MK/GameData/MKPlayerData.h"
#include "../MK/GameData/MKGameBackgroundData.h"
#include "../MK/GameData/MKGameDataLoader.h"
#include "../MK/Common/MKAssertions.h"

// Include GT
#include "../GT/Animation/GTAnimationHandlerNode.h"
#include "../GT/Animation/GTAnimTransAct.h"
#include "../GT/Audio/GTSimperMusicSys.h"
#include "../GT/GameLogic/GTCharacterStatNode.h"
#include "../GT/GameLogic/Obstacle/GTObstacleNode.h"
#include "../GT/GameLogic/PowerUp/GTPowerUp.h"
#include "../GT/Actions/GTFollowNodeAction.h"
#include "../GT/Actions/GTRepeatActionInstantForever.h"

#include "../GT/GameLogic/Powerup/GTSlowTimePowerUp.h"

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

    // Load Player Data
    MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>()->LoadData();

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

    // Create Obstacle & PowerUps Spawner
    InitialiseObstacles();
	InitialisePowerUps();

    // Initialise Camera
    InitialiseCamera();
#ifndef WIN32
#ifdef SDKBOX_ENABLED
    sdkbox::PluginFacebook::setListener(this);
#endif
#endif
	return true;
}

void GameScene::update(float _deltaTime)
{
    // TODO: change this when we have time
    if (m_CharaStatNode->getCurrentState() == DEAD)
    {
        // create all of the UI needed!
        InitialiseGameOverUI();
    }
    else
    {
        ScrollBackgrounds(_deltaTime);
    }

    m_ObstacleSpawner->Update(_deltaTime);
	m_PowerUpSpawner->Update(_deltaTime);
    UpdateUINode();
    UpdateText();
}

void GameScene::onEnter()
{
    Super::onEnter();

    if (m_ObstacleSpawner) { m_ObstacleSpawner->ResumeAllObstacles(); }
	if (m_PowerUpSpawner) { m_PowerUpSpawner->ResumeAllPowerUps(); }

    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_GAMESCENE);
}

void GameScene::onExit()
{
    Super::onExit();

    if (m_ObstacleSpawner) { m_ObstacleSpawner->PauseAllObstacles(); }
	if (m_PowerUpSpawner) { m_PowerUpSpawner->PauseAllPowerUps(); }

    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_DEFAULT);
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
    gtF32 desiredObstacleScale = (visibleSize.height * 0.25f) / playerSprite->getContentSize().height;
    playerSprite->setScale(desiredObstacleScale);
    addChild(playerSprite);

    // Create player animation.
    GTAnimationHandlerNode* playerAnimationHandler = GTAnimationHandlerNode::create();
    //playerAnimationHandler->initWithJSON_tag("SpriteAnim/MainCharaData.json");
    FileUtils::getInstance()->addSearchPath("SpriteAnim");
    playerAnimationHandler->initWithJSON_tag("SpriteAnim/MainCharaData.json");
    playerSprite->addChild(playerAnimationHandler);

    // Create player physics.
    Size playerSpriteContentSize = Size(playerSprite->getContentSize().width * 0.6f, playerSprite->getContentSize().height * 0.8f);
    PhysicsBody* playerPhysicsBody = PhysicsBody::createBox(playerSpriteContentSize);
    playerPhysicsBody->setCategoryBitmask(GT_COLLISION_CATEGORY_PLAYER);
    playerPhysicsBody->setCollisionBitmask(GT_COLLISION_CATEGORY_GROUND);
    playerPhysicsBody->setContactTestBitmask(GT_COLLISION_CATEGORY_GROUND | GT_COLLISION_CATEGORY_OBSTACLE | GT_COLLISION_CATEGORY_POWERUP);
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

    m_CharaStatNode->PassInvokeFunctionWhenResetDistance([&](float _dist) { m_ObstacleSpawner->MoveAllObstacles(_dist); });
    m_CharaStatNode->PassInvokeFunctionWhenResetDistance([&](float _dist) { m_PowerUpSpawner->MoveAllPowerUps(_dist); });
    //m_CharaStatNode->PassInvokeFunctionWhenResetDistance([&](float _dist) { UpdateCamera(); });
    m_CharaStatNode->PassInvokeFunctionWhenResetDistance([&](float _dist) { UpdateText(); });
    m_CharaStatNode->PassInvokeFunctionWhenResetDistance([&](float _dist) { UpdateUINode(); });

    m_CharaStatNode->setResetDistance(m_Ground->getScaleX() * 5.0f);

    auto runResetActionPtr = CallFunc::create(CC_CALLBACK_0(GTCharacterStatNode::ResetPlayerDistance, m_CharaStatNode));
    m_CharaStatNode->runAction(GTRepeatActionInstantForever::Create(runResetActionPtr));

	def = UserDefault::getInstance();

	m_HighScore = def->getIntegerForKey("HIGHSCORE", 0);
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

	m_Backgrounds = new MKSprite*[MKShopItem_Background::BackgroundLayers::NUM_BACKGROUND_LAYERS];
	for (unsigned int i = 0; i < MKShopItem_Background::BackgroundLayers::NUM_BACKGROUND_LAYERS; ++i) { m_Backgrounds[i] = nullptr; }

    // Get the background that the player has currently equipped.
    MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
    MKGameBackgroundData* gameBackgroundData = MKGameDataLoader::GetInstance()->GetGameData<MKGameBackgroundData>();
    MKShopItem_Background* backgroundItem = gameBackgroundData->GetBackground(playerData->GetEquippedBackground());
    MK_ASSERT((backgroundItem != nullptr));

	m_Backgrounds[MKShopItem_Background::BackgroundLayers::STATIC] = MKSprite::Create(backgroundItem->GetBackgroundFile(MKShopItem_Background::STATIC), true);
	m_Backgrounds[MKShopItem_Background::BackgroundLayers::REAR] = MKSprite::Create(backgroundItem->GetBackgroundFile(MKShopItem_Background::REAR), true);
	m_Backgrounds[MKShopItem_Background::BackgroundLayers::MIDDLE] = MKSprite::Create(backgroundItem->GetBackgroundFile(MKShopItem_Background::MIDDLE), true);
	m_Backgrounds[MKShopItem_Background::BackgroundLayers::FRONT] = MKSprite::Create(backgroundItem->GetBackgroundFile(MKShopItem_Background::FRONT), true);

	for (unsigned int i = 0; i < MKShopItem_Background::BackgroundLayers::NUM_BACKGROUND_LAYERS; ++i)
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
		},
		(0.075f * visibleSize.height) / pauseButtonSprite->getContentSize().height
    );
    GetUINode()->addChild(PauseButton);
}

void GameScene::InitialiseText()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    gtF32 desiredObstacleScale = (visibleSize.height * 28.0f) / this->getContentSize().height;
    m_ScoreTxt = Label::createWithTTF("HighScore", "fonts/Marker_Felt.ttf", desiredObstacleScale);
    m_ScoreTxt->setTextColor(Color4B::BLACK);
    m_ScoreTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    m_ScoreTxt->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.9f);
    GetUINode()->addChild(m_ScoreTxt);

	// Timer Text For Slow Time
	gtF32 desiredTextScale = (visibleSize.height * 24.0f) / this->getContentSize().height;
	m_SlowTimerTxt = Label::createWithTTF("Slow Timer:", "fonts/Marker_Felt.ttf", desiredTextScale);
	m_SlowTimerTxt->setTextColor(Color4B::BLACK);
	m_SlowTimerTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_SlowTimerTxt->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.8f);
	m_SlowTimerTxt->setVisible(false);
	GetUINode()->addChild(m_SlowTimerTxt);

	// HighScore Text 
	m_HighScoreTxt = Label::createWithTTF("HighScore", "fonts/Marker_Felt.ttf", desiredObstacleScale);
	m_HighScoreTxt->setTextColor(Color4B::RED);
	m_HighScoreTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_HighScoreTxt->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.75f);
	m_HighScoreTxt->setVisible(false);
	GetUINode()->addChild(m_HighScoreTxt);
}

void GameScene::InitialiseGameOverUI()
{
    // need to ensure that the array of GameOverUI is empty!
    if (m_ArrayOfGameOverUI.size() == 0)
    {
        Sprite* buttonSprite = Sprite::create("ButtonNormal.png");
        Sprite* facebookButtonSprite = Sprite::create("FacebookButton.png");
        m_ObstacleSpawner->PauseAllObstacles();
		m_PowerUpSpawner->PauseAllPowerUps();
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
			Director::getInstance()->getScheduler()->setTimeScale(1.0f);
			this->getPhysicsWorld()->setSpeed(1.0f);
            MKSceneManager::GetInstance()->ReplaceScene("GameOverScene");
        },
			(0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
        GetUINode()->addChild(RetryButton);
        m_ArrayOfGameOverUI.push_back(RetryButton);
        //SETTINGS BUTTON//
        auto ReviveButton = MKUICreator::GetInstance()->createButton(
            Vec2(UIButtonPosX, UIButtonPosY - (RetryButton->getContentSize().height * RetryButton->getScale())),
            "ButtonNormal.png",
            "ButtonSelected.png",
            "Revive: " + std::to_string(m_CharaStatNode->getReviveCounter()),
            [&](Ref*) -> void
        {
            if (m_CharaStatNode->getReviveCounter() > 0)
            {
				m_HighScoreTxt->setVisible(false);
                // have to clear the buttons
                ClearGameOverUI();
                // then revive the player!
                m_CharaStatNode->setState(REVIVE);
                m_ObstacleSpawner->ResumeAllObstacles();
				m_PowerUpSpawner->ResumeAllPowerUps();
            }
        },
			(0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
        GetUINode()->addChild(ReviveButton);
        m_ArrayOfGameOverUI.push_back(ReviveButton);
        //MAIN MENU BUTTON//
        auto ToMainMenuButton = MKUICreator::GetInstance()->createButton(
            Vec2(UIButtonPosX, UIButtonPosY - (RetryButton->getContentSize().height * RetryButton->getScale() * 2)),
            "ButtonNormal.png",
            "ButtonSelected.png",
            "Main Menu",
            [&](Ref*) -> void
        {
            MKSceneManager::GetInstance()->ReplaceScene("GameScene");
        },
			(0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
        GetUINode()->addChild(ToMainMenuButton);
        m_ArrayOfGameOverUI.push_back(ToMainMenuButton);

        auto FacebookButton = MKUICreator::GetInstance()->createButton(
			Vec2(UIButtonPosX, UIButtonPosY - (RetryButton->getContentSize().height * RetryButton->getScale() * 3)),
			"FacebookButton.png",
			"FacebookButtonSelected.png",
			"",
			[&](Ref*) -> void
			{
#ifndef WIN32
#ifdef SDKBOX_ENABLED
                //DO FB LOGIC HERE
                if (sdkbox::PluginFacebook::isLoggedIn())
                {
                    //sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});
                    ShareHighScoreOnFB();
                }
                else
                {
                    sdkbox::PluginFacebook::login();
                }
#endif
#endif
			},
			(0.1f * visibleSize.height) / facebookButtonSprite->getContentSize().height
			);
		GetUINode()->addChild(FacebookButton);
		m_ArrayOfGameOverUI.push_back(FacebookButton);

		if (m_CharaStatNode->getConvertedDistWalk() > m_HighScore)
		{
			m_HighScore = m_CharaStatNode->getConvertedDistWalk();

			def->setIntegerForKey("HIGHSCORE", m_HighScore);
		}	

		std::string HighScoreString = "HighScore: " + std::to_string(m_HighScore);
		m_HighScoreTxt->setString(HighScoreString);
		m_HighScoreTxt->setVisible(true);
    }
}

void GameScene::InitialiseObstacles()
{
    DeinitialiseObstacles();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    m_ObstacleSpawner = new GTObstacleSpawner(this, m_PlayerNode, m_CharaStatNode->getSpeedX(), visibleSize.height * 4.0f);
}

void GameScene::InitialisePowerUps()
{
	DeinitialisePowerUps();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	m_PowerUpSpawner = new GTPowerUpSpawner(this, m_PlayerNode, visibleSize.height * 1.0f, m_ObstacleSpawner);
}

void GameScene::InitialiseCamera()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    GTFollowNodeAction* followAction = GTFollowNodeAction::Create(0.0f, m_PlayerNode, GTFollowNodeAction::X, cocos2d::Vec2(visibleSize.width * 0.3f, 0.0f));
    getDefaultCamera()->runAction(RepeatForever::create(followAction));
}

// Update
void GameScene::ScrollBackgrounds(float _deltaTime)
{
    m_Backgrounds[MKShopItem_Background::BackgroundLayers::REAR]->OffsetTexture(_deltaTime * 0.05f, 0.0f);
	m_Backgrounds[MKShopItem_Background::BackgroundLayers::MIDDLE]->OffsetTexture(_deltaTime * 0.075f, 0.0f);
	m_Backgrounds[MKShopItem_Background::BackgroundLayers::FRONT]->OffsetTexture(_deltaTime * 0.1, 0.0f);
}

void GameScene::UpdateUINode()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_UINode->setPosition(getDefaultCamera()->getPositionX() - visibleSize.width * 0.5f, getDefaultCamera()->getPositionY() - visibleSize.height * 0.5f);
}

void GameScene::UpdateText()
{
    std::string CurrentScoreString = "Score: " + std::to_string(m_CharaStatNode->getConvertedDistWalk());
	m_ScoreTxt->setString(CurrentScoreString);

	if (GTSlowTimePowerUp::m_OnContact)
	{
		std::string slowTimerString = "Slow Time: " + std::to_string((gtU32)GTSlowTimePowerUp::m_currentCountDownTimer);
		m_SlowTimerTxt->setString(slowTimerString);
		m_SlowTimerTxt->setVisible(true);
	}
	if (!GTSlowTimePowerUp::m_OnContact)
	{
		m_SlowTimerTxt->setVisible(false);
	}
}

// Deinitialisation
void GameScene::Deinitialise()
{
	DeinitialiseInput();
    DeinitialiseObstacles();
	DeinitialisePowerUps();
    ClearGameOverUI();
}

void GameScene::DeinitialiseObstacles()
{
    if (m_ObstacleSpawner != nullptr)
    {
        delete m_ObstacleSpawner;
    }
}

void GameScene::DeinitialisePowerUps()
{
	if (m_PowerUpSpawner != nullptr)
	{
		delete m_PowerUpSpawner;
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
    MKInputButton* input = static_cast<MKInputButton*>(_event->getUserData());
    if (input->m_ButtonState == MinamiKotori::MKInputButton::ButtonState::PRESS)
    {
        switch (input->m_InputName)
        {
        case MinamiKotori::MKInputName::JUMP:
            m_CharaStatNode->CharJump();
            break;
        case MKInputName::SLIDE:
            m_CharaStatNode->setState(CHARACTER_STATE::SLIDE);
            break;
        default:
            break;
        }
    }
}

void GameScene::OnClick(EventCustom * _event)
{
    MKInputClick* input = static_cast<MKInputClick*>(_event->getUserData());

    if (input->m_InputName == MinamiKotori::MKInputName::SLIDE)
    {
        SlideInput(input);
    }

    if (input->m_InputName == MinamiKotori::MKInputName::JUMP)
    {
        JumpInput(input);
    }
}

void GameScene::OnAxis(EventCustom * _event)
{
    MKInputAxis* input = static_cast<MKInputAxis*>(_event->getUserData());
}

// Player Controls
void GameScene::SlideInput(const MKInputClick* _input)
{
    switch (_input->m_ButtonState)
    {
    case MinamiKotori::MKInputButton::ButtonState::PRESS:
        m_SlideClickStartPosition = _input->m_CursorPosition;
        m_SlideClickCurrentPosition = _input->m_CursorPosition;
        break;
    case MinamiKotori::MKInputButton::ButtonState::HOLD:
        m_SlideClickCurrentPosition = _input->m_CursorPosition;
        break;
    case MinamiKotori::MKInputButton::ButtonState::RELEASE:
        m_SlideActionReleased = true;
        return;
    default:
        break;
    }

    // Ensure that the delta movement of the click is greater than the deadZone.
    // We only care about the vertical movement.
    mkF32 movementDelta = m_SlideClickCurrentPosition.GetY() - m_SlideClickStartPosition.GetY();
    if (MKMathsHelper::Abs(movementDelta) < m_SwipeDeadZone) { return; }
    if (movementDelta > 0.0f) { return; }

    if (!m_SlideActionReleased) { return; }
    m_SlideActionReleased = false;

    m_CharaStatNode->setState(CHARACTER_STATE::SLIDE);
}

void GameScene::JumpInput(const MKInputClick* _input)
{
    switch (_input->m_ButtonState)
    {
    case MinamiKotori::MKInputButton::ButtonState::PRESS:
        m_JumpClickStartPosition = _input->m_CursorPosition;
        m_JumpClickCurrentPosition = _input->m_CursorPosition;
        break;
    case MinamiKotori::MKInputButton::ButtonState::HOLD:
        m_JumpClickCurrentPosition = _input->m_CursorPosition;
        break;
    case MinamiKotori::MKInputButton::ButtonState::RELEASE:
        m_JumpActionReleased = true;
        return;
    default:
        break;
    }

    // Ensure that the delta movement of the click is greater than the deadZone.
    // We only care about the vertical movement.
    mkF32 movementDelta = m_JumpClickCurrentPosition.GetY() - m_JumpClickStartPosition.GetY();
    if (MKMathsHelper::Abs(movementDelta) < m_SwipeDeadZone) { return; }
    if (movementDelta < 0.0f) { return; }

    if (!m_JumpActionReleased) { return; }
    m_JumpActionReleased = false;

    m_CharaStatNode->CharJump();
}

#ifndef WIN32
#ifdef SDKBOX_ENABLED
void GameScene::onLogin(bool isLogin, const std::string& msg)
{
    // this means the player has login. only when he/she is login then sharing can occur
    if (isLogin)
    {
        ShareHighScoreOnFB();
    }
}
void GameScene::onSharedSuccess(const std::string& message)
{
    CCLOG("Sharing is successful according to %s", message.c_str());
}
void GameScene::onSharedFailed(const std::string& message)
{
    CCLOG("Sharing is failed. plz fix this bug!");
}
void GameScene::onSharedCancel()
{
    CCLOG("sharing is cancel");
}
void GameScene::onAPI(const std::string& key, const std::string& jsonData)
{
    
}
void GameScene::onPermission(bool isLogin, const std::string& msg)
{
    if (isLogin)
    {
        bool needPermissionForShare = true;
        for (std::vector<std::string>::iterator it = sdkbox::PluginFacebook::getPermissionList().begin(), end = sdkbox::PluginFacebook::getPermissionList().end(); it != end; ++it)
        {
            if ((*it) == sdkbox::FB_PERM_PUBLISH_POST)
            {
                needPermissionForShare = false;
                break;
            }
        }
        if (needPermissionForShare)
            sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});
        //ShareHighScoreOnFB();
    }
}
void GameScene::onFetchFriends(bool ok, const std::string& msg)
{
    
}
void GameScene::onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends )
{
    
}
void GameScene::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
    
}
void GameScene::onInviteFriendsResult( bool result, const std::string& msg )
{
    
}

void GameScene::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
{
    
}

void GameScene::ShareHighScoreOnFB()
{
    sdkbox::FBShareInfo info;
    info.type  = sdkbox::FB_LINK;
    info.link = "https://runningfromtripleprog.appspot.com";
    info.title = "Running From Triple Programming!";
    info.text  = "Jialet liao! I only score " + std::to_string(m_HighScore);
    info.image = "http://cocos2d-x.org/images/logo.png";
    sdkbox::PluginFacebook::dialog(info);
}
#endif
#endif
