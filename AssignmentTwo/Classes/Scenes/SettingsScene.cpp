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
#include "SettingsScene.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
using namespace GinTama;

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
	if (!Super::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

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

	int masterVolume = GinTama::GTSimperMusicSys::GetInstance()->getMasterVol() * 100;

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
				GinTama::GTSimperMusicSys::GetInstance()->setMasterVol((float)percent / 100);
			}
		}
	);
	slider->setPercent(masterVolume);
	this->addChild(slider);

	auto label = Label::createWithTTF("Settings Scene", "fonts/Marker_Felt.ttf", 24);
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

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("PlaceHolder/sprite.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("PlaceHolder/sprite_ani.plist");

	scheduleUpdate();

	return true;
}
