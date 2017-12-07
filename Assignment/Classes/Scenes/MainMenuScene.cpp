// Include Cocos
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"
#include "GT/Animation/AnimationHandlerNode.h"
#include "GT/Animation/AnimTransAct.h"
#include "GT/Audio/SimperMusicSys.h"

// Include MK
#include "MK/SceneManagement/MKSceneManager.h"
#include "MK/Common/MKMacros.h"

// Include Input Device Handlers
#include "MK/Input/MKKeyboardHandler.h"

// Include Assignment
#include "AvailableScenes.h"
#include "AudioEngine.h"
#include "external/json/document.h"
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"

//#include "ui/UIButton.h"
#include "UIClass/UICreator.h"
#include "MainMenuScene.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
using namespace GinTama;

MainMenuScene::MainMenuScene()
{
}

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
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	// Trying to figure out the Lua stuff

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	InitialiseSkyBackground();

	// Input Testing
	//InitialiseInput();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	auto toGameButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width / 2, origin.y + visibleSize.height / 2),
		"ButtonNormal.png", 
		"ButtonSelected.png",
		"Play Game",
		[&](Ref*) -> void
		{
        //DeinitialiseInput();
        CCLOG("Go to GameScene");
        MKSceneManager::GetInstance()->ReplaceScene("GameScene");
		}
	);
	this->addChild(toGameButton);

	auto toSettingsButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width / 2, origin.y + visibleSize.height / 2 - toGameButton->getContentSize().height),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Settings",
		[&](Ref*) -> void
		{
            MKSceneManager::GetInstance()->PushScene("SettingsScene");
		}
	);
	this->addChild(toSettingsButton);

	auto exitButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width / 2, origin.y + visibleSize.height / 2 - (2 * toGameButton->getContentSize().height)),
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

	auto label = Label::createWithTTF("Main Menu Scene", "fonts/Marker Felt.ttf", 24);
	if (label == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
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

	GinTama::SimperMusicSys::GetInstance()->playSound("testbgm");

	return true;
}

void MainMenuScene::InitialiseSkyBackground() {}
/*
void MainMenuScene::InitialiseSkyBackground()
{
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_SkyBackground = MKSprite::create("Environment/Backgrounds/Background_Sky.png", true);
	m_SkyBackground->setAnchorPoint(Vec2(0.0f, 0.0f));
	m_SkyBackground->setPosition(visibleOrigin.x, visibleOrigin.y);

	float desiredHeight = visibleSize.height;
	float backgroundHeight = m_SkyBackground->getContentSize().height;
	float desiredScale = desiredHeight / backgroundHeight;
	m_SkyBackground->setScale(desiredScale, desiredScale);

	float backgroundWidth = m_SkyBackground->getContentSize().width;
	float numRepeat = visibleSize.width / (backgroundWidth * desiredScale);
	m_SkyBackground->SetRepeat(numRepeat, 1.0f);

	addChild(m_SkyBackground);
}
*/

void MainMenuScene::OnButton(EventCustom* _event)
{
	MKInputButton* buttonEvent = static_cast<MKInputButton*>(_event->getUserData());

	std::string inputName;
	switch (buttonEvent->m_InputName)
	{
	case MinamiKotori::MKInputName::JUMP:
		inputName = "Jump";
		break;
	case MinamiKotori::MKInputName::SLIDE:
		inputName = "Slide";
		break;
	case MinamiKotori::MKInputName::SMASH:
		inputName = "Smash";
		break;
	default:
		inputName = "Unknown InputName";
		break;
	}

	std::string buttonState;
	switch (buttonEvent->m_ButtonState)
	{
	case MinamiKotori::MKInputButton::ButtonState::PRESS:
		buttonState = "Pressed";
		break;
	case MinamiKotori::MKInputButton::ButtonState::HOLD:
		buttonState = "Held";
		break;
	case MinamiKotori::MKInputButton::ButtonState::RELEASE:
		buttonState = "Released";
		break;
	default:
		buttonState = "Unknown ButtonState";
		break;
	}

	std::string logMessage = inputName + " " + buttonState;
	//CCLOG(logMessage.c_str());
	m_InputLabel->setString(logMessage);
}

void MainMenuScene::OnClick(EventCustom* _event)
{
	MKInputClick* clickEvent = static_cast<MKInputClick*>(_event->getUserData());

	std::string inputName;
	switch (clickEvent->m_InputName)
	{
	case MinamiKotori::MKInputName::JUMP:
		inputName = "Jump";
		break;
	case MinamiKotori::MKInputName::SLIDE:
		inputName = "Slide";
		break;
	case MinamiKotori::MKInputName::SMASH:
		inputName = "Smash";
		break;
	default:
		inputName = "Unknown InputName";
		break;
	}

	std::string buttonState;
	switch (clickEvent->m_ButtonState)
	{
	case MinamiKotori::MKInputButton::ButtonState::PRESS:
		buttonState = "Pressed";
		break;
	case MinamiKotori::MKInputButton::ButtonState::HOLD:
		buttonState = "Held";
		break;
	case MinamiKotori::MKInputButton::ButtonState::RELEASE:
		buttonState = "Released";
		break;
	default:
		buttonState = "Unknown ButtonState";
		break;
	}

	std::string logMessage = inputName + " " + buttonState;
	//CCLOG(logMessage.c_str());
	m_InputLabel->setString(logMessage);
}

void MainMenuScene::OnAxis(EventCustom* _event)
{
	MKInputAxis* axisEvent = static_cast<MKInputAxis*>(_event->getUserData());

	std::string inputName;
	switch (axisEvent->m_InputName)
	{
	case MinamiKotori::MKInputName::JUMP:
		inputName = "Jump";
		break;
	case MinamiKotori::MKInputName::SLIDE:
		inputName = "Slide";
		break;
	case MinamiKotori::MKInputName::SMASH:
		inputName = "Smash";
		break;
	default:
		inputName = "Unknown InputName";
		break;
	}

	std::string axisValue;
	axisValue += axisEvent->m_AxisValue;

	std::string logMessage = inputName + " " + axisValue;
	//CCLOG(logMessage.c_str());
	m_InputLabel->setString(logMessage);
}

void MainMenuScene::update(float _deltaTime)
{
	m_SceneChangeCounter -= _deltaTime;
	if (m_SceneChangeCounter < 0.0f)
	{
		//MKSceneManager::GetInstance()->ReplaceScene("MKSceneDerived");
		//MKSceneManager::GetInstance()->PushScene("MKSceneDerived");
		m_SceneChangeCounter = 3.0f;
	}
}
