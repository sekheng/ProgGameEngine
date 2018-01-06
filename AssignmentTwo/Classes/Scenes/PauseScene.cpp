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

//#include "ui/UIButton.h"
#include "../UIClass/UICreator.h"
#include "PauseScene.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
using namespace GinTama;

PauseScene::PauseScene()
{
}

PauseScene::~PauseScene()
{
}

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
	if (!Scene::init())
	{
		return false;
	}

	// Trying to figure out the Lua stuff

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	InitializePauseMenuButtons();

	auto label = Label::createWithTTF("Pause", "Fonts/Marker_Felt.ttf", 24);
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

	return true;
}

void PauseScene::InitializePauseMenuButtons()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//RESUME BUTTON//
	auto toPrevSceneButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Resume",
		[&](Ref*) -> void
		{
			//DeinitialiseInput();
			MKSceneManager::GetInstance()->PopScene();
		}
	);
	this->addChild(toPrevSceneButton);

	//SETTINGS BUTTON//
	auto ToSettingsButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f - toPrevSceneButton->getContentSize().height),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Settings",
		[&](Ref*) -> void
		{
			//DeinitialiseInput();
			MKSceneManager::GetInstance()->PushScene("SettingsScene");
		}
	);
	this->addChild(ToSettingsButton);

	//MAIN MENU BUTTON//
	auto ToMainMenuButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f - toPrevSceneButton->getContentSize().height * 2),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Main Menu",
		[&](Ref*) -> void
		{
			//DeinitialiseInput();
			MKSceneManager::GetInstance()->ReplaceScene("MainMenuScene");
		}
	);
	this->addChild(ToMainMenuButton);
}

void PauseScene::OnButton(EventCustom* _event)
{
}

void PauseScene::OnClick(EventCustom* _event)
{
}

void PauseScene::OnAxis(EventCustom* _event)
{
}

void PauseScene::update(float _deltaTime)
{
}