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

	InitialiseSkyBackground();

	Sprite* backButton = Sprite::create("BackButton.png");

	auto toPrevSceneButton = MKUICreator::GetInstance()->createButton(
		Vec2(backButton->getContentSize().width, visibleSize.height - backButton->getContentSize().height),
		"BackButton.png",
		"BackButton.png",
		"",
		[&](Ref*) -> void
	{
		//DeinitialiseInput();
		MKSceneManager::GetInstance()->PopScene();
	},
		1.0f
		);
	this->addChild(toPrevSceneButton);

	int masterVolume = GinTama::GTSimperMusicSys::GetInstance()->getMasterVol() * 100;

	auto slider = MKUICreator::GetInstance()->createSlider(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f),
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

void SettingsScene::InitialiseSkyBackground()
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
