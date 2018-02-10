// Include Cocos
#include "../GT/Audio/GTSimperMusicSys.h"

// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"
#include "../MK/Common/MKMacros.h"
#include "../MK/GameData/MKGameDataLoader.h"

// Include Assignment
#include "AvailableScenes.h"

#include "../UIClass/UICreator.h"
#include "MainMenuScene.h"
#include "../GT/Facebook/GTFacebookHelper.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
USING_NS_GT

static int BGM_ID = -1;

MainMenuScene::~MainMenuScene()
{
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MainMenuScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
	if (!Super::init()) { return false; }
    
    // Coins (For Debuging Purposes. Will Remove Soon.)
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
    auto label = Label::createWithTTF("Coins: " + std::to_string(playerData->GetCoins()), "fonts/Marker_Felt.ttf", 24);
    // position the label on the center of the screen
    label->setPosition(Vec2(visibleOrigin.x + visibleSize.width * 0.5f,
        visibleOrigin.y + visibleSize.height - label->getContentSize().height * 2.0f));
    
    // add the label as a child to this layer
    this->addChild(label, 1);

    InitialiseBackground();
    InitialiseUI();
    InitialiseFacebookUI();

    // Play BGM
    if (!GTSimperMusicSys::GetInstance()->isPlayingSound(BGM_ID))
    {
        BGM_ID = GTSimperMusicSys::GetInstance()->playSound("testbgm");
    }

    scheduleUpdate();

	return true;
}

void MainMenuScene::InitialiseBackground()
{
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_Background = MKSprite::Create("Textures/Backgrounds/Background_Menu.png", true);
    m_Background->setAnchorPoint(Vec2(0.0f, 0.0f));
    m_Background->setPosition(visibleOrigin.x, visibleOrigin.y);

    // We want the background to fill up the whole screen.
    float backgroundWidth = m_Background->getContentSize().width;
    float backgroundHeight = m_Background->getContentSize().height;
    float backgroundAspectRatio = backgroundWidth / backgroundHeight;

    float desiredWidth = visibleSize.width;
    float desiredHeight = visibleSize.height;

    m_Background->setScale(desiredWidth / backgroundWidth, desiredHeight / backgroundHeight);
    m_Background->SetTextureScale(backgroundWidth / desiredWidth, 1.0f);

    addChild(m_Background);
}

void MainMenuScene::InitialiseUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float halfWidth = visibleSize.width * 0.5f;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float estimatedHalfPoint = origin.x + halfWidth;

    // Title
    auto label = Label::createWithTTF("Main Menu Scene", "fonts/Marker_Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'Fonts/Marker_Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + halfWidth,
            origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    Sprite* buttonSprite = Sprite::create("ButtonNormal.png");

    // Start Game Button
    auto toGameButton = MKUICreator::GetInstance()->createButton(
        Vec2(estimatedHalfPoint, visibleSize.height * 0.6f),
        "ButtonNormal.png",
        "ButtonSelected.png",
        "Play Game",
        [&](Ref*) -> void
    {
        MKSceneManager::GetInstance()->ReplaceScene("GameScene");
    },
        (0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
    ++m_NumberOfUIAtMain;
    this->addChild(toGameButton);

    // Settings Button
    auto toSettingsButton = MKUICreator::GetInstance()->createButton(
        Vec2(estimatedHalfPoint, (visibleSize.height * 0.6f) - (m_NumberOfUIAtMain * buttonSprite->getContentSize().height * toGameButton->getScaleY())),
        "ButtonNormal.png",
        "ButtonSelected.png",
        "Settings",
        [&](Ref*) -> void
    {
        MKSceneManager::GetInstance()->PushScene("SettingsScene");
    },
        (0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
    this->addChild(toSettingsButton);
    ++m_NumberOfUIAtMain;
    // Shop Button
    auto toShopButton = MKUICreator::GetInstance()->createButton(
        Vec2(estimatedHalfPoint, (visibleSize.height * 0.6f) - (m_NumberOfUIAtMain * buttonSprite->getContentSize().height * toGameButton->getScaleY())),
        "ButtonNormal.png",
        "ButtonSelected.png",
        "Shop",
        [&](Ref*) -> void
    {
        MKSceneManager::GetInstance()->PushScene("ShopScene");
    },
        (0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
    this->addChild(toShopButton);
    ++m_NumberOfUIAtMain;

    auto toHighscoreButton = MKUICreator::GetInstance()->createButton(
        Vec2(estimatedHalfPoint, (visibleSize.height * 0.6f) - (m_NumberOfUIAtMain * buttonSprite->getContentSize().height * toGameButton->getScaleY())),
        "ButtonNormal.png",
        "ButtonSelected.png",
        "Highscore",
        [&](Ref*) -> void
    {
        MKSceneManager::GetInstance()->PushScene("HighscoreScene");
    },
        (0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
    this->addChild(toHighscoreButton);
    ++m_NumberOfUIAtMain;

    // Exit Game Button
    auto exitButton = MKUICreator::GetInstance()->createButton(
        Vec2(estimatedHalfPoint, (visibleSize.height * 0.6f) - (m_NumberOfUIAtMain * buttonSprite->getContentSize().height * toGameButton->getScaleY())),
        "ButtonNormal.png",
        "ButtonSelected.png",
        "Exit",
        [](Ref*) -> void
    {
        Director::getInstance()->end();

        // Destroy the singletons.
        MKInputManager::GetInstance()->Destroy();
        MKSceneManager::GetInstance()->Destroy();
        MKGameDataLoader::GetInstance()->Destroy();
    },
        (0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
    this->addChild(exitButton);
    ++m_NumberOfUIAtMain;
}

void MainMenuScene::InitialiseFacebookUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
    float halfWidth = visibleSize.width * 0.5f;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float estimatedHalfPoint = origin.x + halfWidth;
    
	Sprite* buttonSprite = Sprite::create("ButtonNormal.png");
	Sprite* FBLoginbuttonSprite = Sprite::create("FacebookLoginButton.png");

	m_FacebookLoginButton = MKUICreator::GetInstance()->createButton(
		Vec2(estimatedHalfPoint, (visibleSize.height * 0.6f) - (m_NumberOfUIAtMain * buttonSprite->getContentSize().height * (0.1f * visibleSize.height) / buttonSprite->getContentSize().height)),
		"FacebookLoginButton.png",
		"FacebookLoginButtonSelected.png",
		"",
		[&](Ref*) -> void
		{
#ifndef WIN32
            if (!sdkbox::PluginFacebook::isLoggedIn())
            {
                sdkbox::PluginFacebook::login();
            }
#else
            ToggleFacebookUI(true);
#endif
        },
		(0.1f * visibleSize.height) / FBLoginbuttonSprite->getContentSize().height
		);
    m_FacebookLoginButton->setVisible(true);
	this->addChild(m_FacebookLoginButton);

    m_FacebookLogoutButton = MKUICreator::GetInstance()->createButton(
		Vec2(estimatedHalfPoint, (visibleSize.height * 0.6f) - (m_NumberOfUIAtMain * buttonSprite->getContentSize().height * (0.1f * visibleSize.height) / buttonSprite->getContentSize().height)),
		"FacebookLogoutButton.png",
		"FacebookLogoutButtonSelected.png",
		"",
		[&](Ref*) -> void
		{
			//DO LOGOUT STUFF
#ifndef WIN32
            if (sdkbox::PluginFacebook::isLoggedIn())
            {
                sdkbox::PluginFacebook::logout();
                ToggleFacebookUI(false);
            }
#else
            ToggleFacebookUI(false);
#endif
        },
		(0.1f * visibleSize.height) / FBLoginbuttonSprite->getContentSize().height
		);
    m_FacebookLogoutButton->setVisible(false);
	this->addChild(m_FacebookLogoutButton);
    ++m_NumberOfUIAtMain;
#ifndef WIN32
#ifdef SDKBOX_ENABLED
    ToggleFacebookUI(sdkbox::PluginFacebook::isLoggedIn());
    sdkbox::PluginFacebook::setListener(this);
#endif
#endif
}

void MainMenuScene::ToggleFacebookUI(bool _isLoggedIn)
{
    if (_isLoggedIn)
    {
        m_FacebookLoginButton->setVisible(false);
        m_FacebookLogoutButton->setVisible(true);
    }
    else
    {
        m_FacebookLoginButton->setVisible(true);
        m_FacebookLogoutButton->setVisible(false);
    }
}

#ifndef WIN32
#ifdef SDKBOX_ENABLED
void MainMenuScene::onLogin(bool isLogin, const std::string& msg)
{
    CCLOG("FB login: %u", isLogin);
    bool needPermissionForShare = GTFacebookHelper::CheckForPermissionsNeeded(ALL_PUBLISH_PERMISSIONS);
    if (needPermissionForShare)
        sdkbox::PluginFacebook::requestPublishPermissions(ALL_PUBLISH_PERMISSIONS);
    needPermissionForShare = GTFacebookHelper::CheckForPermissionsNeeded(ALL_READ_PERMISSIONS);
    if (needPermissionForShare)
        sdkbox::PluginFacebook::requestReadPermissions(ALL_READ_PERMISSIONS);
    ToggleFacebookUI(isLogin);
}
void MainMenuScene::onSharedSuccess(const std::string& message)
{
}
void MainMenuScene::onSharedFailed(const std::string& message)
{
}
void MainMenuScene::onSharedCancel()
{
}
void MainMenuScene::onAPI(const std::string& key, const std::string& jsonData)
{
    
}
void MainMenuScene::onPermission(bool isLogin, const std::string& msg)
{
}
void MainMenuScene::onFetchFriends(bool ok, const std::string& msg)
{
    
}
void MainMenuScene::onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends )
{
    
}
void MainMenuScene::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
    
}
void MainMenuScene::onInviteFriendsResult( bool result, const std::string& msg )
{
    
}

void MainMenuScene::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
{
    
}
#endif
#endif
