// Include Cocos
#include "../GT/Audio/GTSimperMusicSys.h"

// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"
#include "../MK/Common/MKMacros.h"

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
	//////////////////////////////
	// 1. super init first
	if (!Super::init())
	{
		return false;
	}

	// Trying to figure out the Lua stuff

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	InitialiseSkyBackground();

	Sprite* buttonSprite = Sprite::create("ButtonNormal.png");
	
	auto toGameButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.6f),
		"ButtonNormal.png", 
		"ButtonSelected.png",
		"Play Game",
		[&](Ref*) -> void
		{
			MKSceneManager::GetInstance()->ReplaceScene("GameScene");
		}
	);
	this->addChild(toGameButton);

	auto toSettingsButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, (visibleSize.height * 0.6f) - (buttonSprite->getContentSize().height * toGameButton->getScaleY())),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Settings",
		[&](Ref*) -> void
		{
            MKSceneManager::GetInstance()->PushScene("SettingsScene");
		}
	);
	this->addChild(toSettingsButton);

	auto toShopButton = MKUICreator::GetInstance()->createButton(
		Vec2((visibleSize.width * 0.5f), (visibleSize.height * 0.6f) - (2 * buttonSprite->getContentSize().height * toGameButton->getScaleY())),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Shop",
		[&](Ref*) -> void
	{
		MKSceneManager::GetInstance()->PushScene("ShopScene");
	}
	);
	this->addChild(toShopButton);

	auto exitButton = MKUICreator::GetInstance()->createButton(
		Vec2((visibleSize.width * 0.5f), (visibleSize.height * 0.6f) - (3 * buttonSprite->getContentSize().height * toGameButton->getScaleY())),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Exit",
		[](Ref*) -> void
		{
			Director::getInstance()->end();
		}
	);
	this->addChild(exitButton);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

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

	scheduleUpdate();
    if (!GTSimperMusicSys::GetInstance()->isPlayingSound(BGM_ID))
    {
        BGM_ID = GTSimperMusicSys::GetInstance()->playSound("testbgm");
    }
	return true;
}

void MainMenuScene::InitialiseSkyBackground()
{
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_SkyBackground = MKSprite::Create("Textures/Backgrounds/Background_Sky.png", true);
    m_SkyBackground->setAnchorPoint(Vec2(0.0f, 0.0f));
    m_SkyBackground->setPosition(visibleOrigin.x, visibleOrigin.y);

    // We want the background to fill up the whole screen.
    float backgroundWidth = m_SkyBackground->getContentSize().width;
    float backgroundHeight = m_SkyBackground->getContentSize().height;
    float backgroundAspectRatio = backgroundWidth / backgroundHeight;

    float desiredWidth = visibleSize.width;
    float desiredHeight = visibleSize.height;

    m_SkyBackground->setScale(desiredWidth / backgroundWidth, desiredHeight / backgroundHeight);
    m_SkyBackground->SetTextureScale(backgroundWidth / desiredWidth, 1.0f);

    addChild(m_SkyBackground);
}
