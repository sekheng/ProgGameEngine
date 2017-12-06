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
#include "SettingsScene.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
using namespace GinTama;

SettingsScene::SettingsScene()
{
	int breakpoint = 0;
	return;
}

SettingsScene::~SettingsScene()
{
	int breakpoint = 0;
	return;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SettingsScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SettingsScene::init()
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

	// Input Testing
	//InitialiseInput();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object

	auto toPrevSceneButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width / 2, visibleSize.height / 5),
		"ButtonNormal.png",
		"ButtonSelected.png",
		"Back",
		[&](Ref*) -> void
		{
        //DeinitialiseInput();
			MKSceneManager::GetInstance()->PopScene();
		}
	);
	this->addChild(toPrevSceneButton);

	int masterVolume = GinTama::SimperMusicSys::GetInstance()->getMasterVol() * 100;

	auto slider = MKUICreator::GetInstance()->createSlider(
		Vec2(visibleSize.width / 2, visibleSize.height / 2),
		"SliderBar.png",
		"ProgressBar.png",
		"SliderBall.png",
		[&](Ref* _sender, ui::Slider::EventType _type) -> void
		{
			ui::Slider* slider = dynamic_cast<ui::Slider*>(_sender);
			if (_type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
			{
				int percent = slider->getPercent();
				GinTama::SimperMusicSys::GetInstance()->setMasterVol((float)percent / 100);
			}
		}
	);
	slider->setPercent(masterVolume);
	this->addChild(slider);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("Settings Scene", "fonts/Marker Felt.ttf", 24);
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

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("PlaceHolder/sprite.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("PlaceHolder/sprite_ani.plist");

	LuaEngine *luaEngine = LuaEngine::getInstance();
	ScriptEngineManager::getInstance()->setScriptEngine(luaEngine);
	lua_State* L = luaEngine->getLuaStack()->getLuaState();
	lua_module_register(L);

	FileUtils::getInstance()->addSearchPath("Resources");
	FileUtils::getInstance()->addSearchPath("PlaceHolder");
	luaEngine->executeScriptFile("DataDriven.lua");
	scheduleUpdate();

	return true;
}

//void SettingsScene::InitialiseInput()
//{
//	Size visibleSize = Director::getInstance()->getVisibleSize();
//	Vec2 origin = Director::getInstance()->getVisibleOrigin();
//	m_InputLabel = Label::createWithTTF("Input Debug Label", "fonts/Marker Felt.ttf", 24);
//	// position the label on the center of the screen
//	m_InputLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
//		origin.y + visibleSize.height - m_InputLabel->getContentSize().height - (visibleSize.height / 4)));
//	this->addChild(m_InputLabel, 1);
//
//	MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT1);
//	m_ButtonListener = MKInputManager::GetInstance()->CreateEventListener<MKInputButton>(CC_CALLBACK_1(SettingsScene::OnButton, this));
//	m_ClickListener = MKInputManager::GetInstance()->CreateEventListener<MKInputClick>(CC_CALLBACK_1(SettingsScene::OnClick, this));
//	m_AxisListener = MKInputManager::GetInstance()->CreateEventListener<MKInputAxis>(CC_CALLBACK_1(SettingsScene::OnAxis, this));
//}

void SettingsScene::OnButton(EventCustom* _event)
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

void SettingsScene::OnClick(EventCustom* _event)
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

void SettingsScene::OnAxis(EventCustom* _event)
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

void SettingsScene::update(float _deltaTime)
{
	m_SceneChangeCounter -= _deltaTime;
	if (m_SceneChangeCounter < 0.0f)
	{
		//MKSceneManager::GetInstance()->ReplaceScene("MKSceneDerived");
		//MKSceneManager::GetInstance()->PushScene("MKSceneDerived");
		m_SceneChangeCounter = 3.0f;
	}
}
