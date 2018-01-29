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
#include "ShopScene.h"

using namespace experimental;
using namespace RAPIDJSON_NAMESPACE;
using namespace GinTama;

static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SettingsScene.cpp\n");
}

bool ShopScene::init()
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

	auto shopScroller = MKUICreator::GetInstance()->createScroller(
		ui::ScrollView::Direction::VERTICAL,
		Size(visibleSize.width, visibleSize.height),
		Size(visibleSize.width, visibleSize.height * 5),
		true,
		Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y)
	);

	for (int i = 0; i < 5; i++)
	{
		ui::Button *button = ui::Button::create("ButtonNormal.png", "ButtonNormal.png");
		button->setPosition( Vec2(shopScroller->getContentSize().width * 0.5f, visibleSize.height * 0.35f + (i * visibleSize.height)) );
		shopScroller->addChild(button);
	}
	this->addChild(shopScroller);

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

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("PlaceHolder/sprite.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("PlaceHolder/sprite_ani.plist");

	scheduleUpdate();

	return true;
}

void ShopScene::InitialiseSkyBackground()
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
