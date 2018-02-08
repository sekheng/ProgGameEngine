// Include Cocos
#include "../GT/Audio/GTSimperMusicSys.h"

// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"
#include "../MK/Common/MKMacros.h"

// Include Assignment
#include "AvailableScenes.h"
#include "AudioEngine.h"

//#include "ui/UIButton.h"
#include "../UIClass/UICreator.h"
#include "PauseScene.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
using namespace GinTama;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in PauseScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PauseScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Super::init())
    {
		return false;
	}

    InitializeTitle();
    InitializePauseMenuButtons();
	scheduleUpdate();

	return true;
}

void PauseScene::InitializeTitle()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("Pause", "fonts/Marker_Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker_Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }
}

void PauseScene::InitializePauseMenuButtons()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* buttonSprite = Sprite::create("ButtonNormal.png");

	//RESUME BUTTON//
	auto toPrevSceneButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Resume",
		[&](Ref*) -> void
		{
			MKSceneManager::GetInstance()->PopScene();
		},
		(0.1f * visibleSize.height) / buttonSprite->getContentSize().height
	);
	this->addChild(toPrevSceneButton);

	//SETTINGS BUTTON//
	auto toSettingsButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f - (toPrevSceneButton->getContentSize().height * toPrevSceneButton->getScale())),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Settings",
		[&](Ref*) -> void
		{
			//DeinitialiseInput();
			MKSceneManager::GetInstance()->PushScene("SettingsScene");
		},
		(0.1f * visibleSize.height) / buttonSprite->getContentSize().height
	);
	this->addChild(toSettingsButton);

	//MAIN MENU BUTTON//
	auto toMainMenuButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f - (toPrevSceneButton->getContentSize().height * toPrevSceneButton->getScale() * 2)),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Main Menu",
		[&](Ref*) -> void
		{
            MKSceneManager::GetInstance()->PopScene();
			MKSceneManager::GetInstance()->ReplaceScene("MainMenuScene");
		},
		(0.1f * visibleSize.height) / buttonSprite->getContentSize().height
	);
	this->addChild(toMainMenuButton);
}