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
#include "GameOverScene.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
using namespace GinTama;

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameOverScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOverScene::init()
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
	
	InitializeLoadingUI();
	//InitializeGameOverMenuButtons();

	//auto label = Label::createWithTTF("GAME OVER", "Fonts/Marker_Felt.ttf", 24);
	//if (label == nullptr)
	//{
	//	problemLoading("'Fonts/Marker_Felt.ttf'");
	//}
	//else
	//{
	//	// position the label on the center of the screen
	//	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
	//		origin.y + visibleSize.height - label->getContentSize().height));

	//	// add the label as a child to this layer
	//	this->addChild(label, 1);
	//}
	scheduleUpdate();
    m_TimeCounter = 0;
	return true;
}

void GameOverScene::InitializeLoadingUI()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	gtF32 desiredObstacleScale = (visibleSize.height * 24.0f) / this->getContentSize().height;
	auto m_FinalHighScoreTxt = Label::createWithTTF("HighScore", "fonts/Marker_Felt.ttf", desiredObstacleScale);
	m_FinalHighScoreTxt->setTextColor(Color4B::WHITE);
	m_FinalHighScoreTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_FinalHighScoreTxt->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.7f);
	m_FinalHighScoreTxt->setString("Loading");
	
	this->addChild(m_FinalHighScoreTxt);
}

void GameOverScene::InitializeGameOverMenuButtons()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//RESUME BUTTON//
	auto RetryButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Retry",
		[&](Ref*) -> void
		{
			//DeinitialiseInput();
			MKSceneManager::GetInstance()->ReplaceScene("GameScene");
		}
	);
	this->addChild(RetryButton);

	//SETTINGS BUTTON//
	auto ReviveButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f - RetryButton->getContentSize().height),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Revive",
		[&](Ref*) -> void
		{
			//DeinitialiseInput();
			MKSceneManager::GetInstance()->PopScene();
		}
	);
	this->addChild(ReviveButton);

	//MAIN MENU BUTTON//
	auto ToMainMenuButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f - RetryButton->getContentSize().height * 2),
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

void GameOverScene::update(float _deltaTime)
{
    m_TimeCounter += _deltaTime;
    if (m_TimeCounter > 0.2f)
    {
        MKSceneManager::GetInstance()->ReplaceScene("GameScene");
    }
}