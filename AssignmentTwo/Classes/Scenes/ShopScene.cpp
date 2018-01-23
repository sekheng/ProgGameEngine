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

	auto shopScroller = MKUICreator::GetInstance()->createScroller(
		ui::ScrollView::Direction::VERTICAL,
		Size(visibleSize.width, visibleSize.height),
		Size(visibleSize.width, visibleSize.height * 5),
		false,
		Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y)
	);

	for (int i = 0; i < 5; i++)
	{
		ui::Button *button = ui::Button::create("ButtonNormal.png", "ButtonNormal.png");
		button->setPosition( Vec2(shopScroller->getContentSize().width * 0.5f, visibleSize.height * 0.35f + (i * visibleSize.height)) );
		shopScroller->addChild(button);
	}
	this->addChild(shopScroller);

	auto toPrevSceneButton = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.15f),
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

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("PlaceHolder/sprite.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("PlaceHolder/sprite_ani.plist");

	scheduleUpdate();

	return true;
}