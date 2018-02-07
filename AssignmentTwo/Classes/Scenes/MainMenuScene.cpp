// Include Cocos
#include "../GT/Audio/GTSimperMusicSys.h"

// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"
#include "../MK/Common/MKMacros.h"
#include "../MK/GameData/MKShopData.h"

// Include Input Device Handlers
#include "../MK/Input/MKKeyboardHandler.h"

// Include Assignment
#include "AvailableScenes.h"
#include "AudioEngine.h"

#include "../UIClass/UICreator.h"
#include "MainMenuScene.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
using namespace GinTama;

static int BGM_ID = -1;

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

    // Do this in Main Menu as it only needs to be loaded once.
    MKShopData::GetInstance()->LoadBackgroundsData();
    
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
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Title
    auto label = Label::createWithTTF("Main Menu Scene", "fonts/Marker_Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'Fonts/Marker_Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    Sprite* buttonSprite = Sprite::create("ButtonNormal.png");
    //Sprite* FBLoginbuttonSprite = Sprite::create("FacebookLoginButton.png");
    //Sprite* FBLoginbuttonSelectedSprite = Sprite::create("FacebookLoginButtonSelected.png");

    // Start Game Button
    auto toGameButton = MKUICreator::GetInstance()->createButton(
        Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.6f),
        "ButtonNormal.png",
        "ButtonSelected.png",
        "Play Game",
        [&](Ref*) -> void
    {
        MKSceneManager::GetInstance()->ReplaceScene("GameScene");
    },
        (0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
    this->addChild(toGameButton);

    // Settings Button
    auto toSettingsButton = MKUICreator::GetInstance()->createButton(
        Vec2(visibleSize.width * 0.5f, (visibleSize.height * 0.6f) - (buttonSprite->getContentSize().height * toGameButton->getScaleY())),
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

    // Shop Button
    auto toShopButton = MKUICreator::GetInstance()->createButton(
        Vec2((visibleSize.width * 0.5f), (visibleSize.height * 0.6f) - (2 * buttonSprite->getContentSize().height * toGameButton->getScaleY())),
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

    // Exit Game Button
    auto exitButton = MKUICreator::GetInstance()->createButton(
        Vec2((visibleSize.width * 0.5f), (visibleSize.height * 0.6f) - (3 * buttonSprite->getContentSize().height * toGameButton->getScaleY())),
        "ButtonNormal.png",
        "ButtonSelected.png",
        "Exit",
        [](Ref*) -> void
    {
        Director::getInstance()->end();
    },
        (0.1f * visibleSize.height) / buttonSprite->getContentSize().height
        );
    this->addChild(exitButton);
}

void MainMenuScene::InitialiseFacebookUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* buttonSprite = Sprite::create("ButtonNormal.png");
	Sprite* FBLoginbuttonSprite = Sprite::create("FacebookLoginButton.png");

	m_FacebookLoginButton = MKUICreator::GetInstance()->createButton(
		Vec2((visibleSize.width * 0.5f), (visibleSize.height * 0.6f) - (4 * buttonSprite->getContentSize().height * (0.1f * visibleSize.height) / buttonSprite->getContentSize().height)),
		"FacebookLoginButton.png",
		"FacebookLoginButtonSelected.png",
		"",
		[&](Ref*) -> void
		{
#ifndef WIN32
#else
        m_FacebookLoginButton->setVisible(false);
        m_FacebookLogoutButton->setVisible(true);
#endif
			//DO LOGIN STUFF
        },
		(0.1f * visibleSize.height) / FBLoginbuttonSprite->getContentSize().height
		);
    m_FacebookLoginButton->setVisible(true);
	this->addChild(m_FacebookLoginButton);

    m_FacebookLogoutButton = MKUICreator::GetInstance()->createButton(
		Vec2((visibleSize.width * 0.5f), (visibleSize.height * 0.6f) - (4 * buttonSprite->getContentSize().height * (0.1f * visibleSize.height) / buttonSprite->getContentSize().height)),
		"FacebookLogoutButton.png",
		"FacebookLogoutButtonSelected.png",
		"",
		[&](Ref*) -> void
		{
			//DO LOGOUT STUFF
#ifndef WIN32
#else
            m_FacebookLoginButton->setVisible(true);
            m_FacebookLogoutButton->setVisible(false);
#endif
        },
		(0.1f * visibleSize.height) / FBLoginbuttonSprite->getContentSize().height
		);
    m_FacebookLogoutButton->setVisible(false);
	this->addChild(m_FacebookLogoutButton);
}