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
#include "../MK/GameData/MKPlayerData.h"

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

	InitialiseBackground();
	InitialiseUI();
	

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("PlaceHolder/sprite.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("PlaceHolder/sprite_ani.plist");

	scheduleUpdate();

	return true;
}

void ShopScene::InitialiseBackground()
{
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_Background = MKSprite::Create("Textures/Backgrounds/Background_Menu.png", true);
	m_Background->setAnchorPoint(Vec2(0.0f, 0.0f));
	m_Background->setPosition(visibleOrigin.x, visibleOrigin.y);

	// We want the background to fill up the whole screen.
	float backgroundWidth = m_Background->getContentSize().width;
	float backgroundHeight = m_Background->getContentSize().height;
	float backgroundAspectRatio = backgroundWidth / backgroundHeight;

	float desiredWidth = visibleSize.width;
	float desiredHeight = visibleSize.height;

	m_Background->setScale(desiredWidth / backgroundWidth, desiredHeight / backgroundHeight);
	m_Background->SetTextureScale(backgroundWidth / desiredWidth, 1.0f);

	addChild(m_Background);
}

void ShopScene::InitialiseUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	Sprite *shopItemButtonSprite = Sprite::create("Textures/UI/City_button.png");

	//ADD SHOPITEM BUTTONS to Vector
	auto cityItem_Button = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f + (visibleSize.height)),
		"Textures/UI/City_button.png",
		"Textures/UI/City_button_Selected.png",
		"",
		[&](Ref*) -> void
		{
			//Do ShopItem stuff here
		},
		(0.75f * visibleSize.height) / shopItemButtonSprite->getContentSize().height
		);
	shopItemButtons.push_back(cityItem_Button);

	auto placeholder0_Item_Button = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f + (visibleSize.height)),
		"Textures/UI/Placeholder0_button.png",
		"Textures/UI/Placeholder0_button_Selected.png",
		"",
		[&](Ref*) -> void
		{
			//Do ShopItem stuff here
		},
		(0.75f * visibleSize.height) / shopItemButtonSprite->getContentSize().height
		);
	shopItemButtons.push_back(placeholder0_Item_Button);

	auto placeholder1_Item_Button = MKUICreator::GetInstance()->createButton(
		Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.5f + (visibleSize.height)),
		"Textures/UI/Placeholder1_button.png",
		"Textures/UI/Placeholder1_button_Selected.png",
		"",
		[&](Ref*) -> void
		{
			//Do ShopItem stuff here
		},
		(0.75f * visibleSize.height) / shopItemButtonSprite->getContentSize().height
		);
	shopItemButtons.push_back(placeholder1_Item_Button);

	//Initialise shopScroller
	auto shopScroller = MKUICreator::GetInstance()->createScroller(
		ui::ScrollView::Direction::VERTICAL,
		Size(visibleSize.width, visibleSize.height),
		Size(visibleSize.width, visibleSize.height * shopItemButtons.size()),
		true,
		Vec2(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y)
	);

	//Loop through the buttons and re-set position of each buttons
	for (int i = 0; i < shopItemButtons.size(); i++)
	{
		shopItemButtons[i]->setPosition(Vec2(shopScroller->getContentSize().width * 0.5f, visibleSize.height * 0.5f + (i * visibleSize.height)));
		shopScroller->addChild(shopItemButtons[i]);
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
		(0.1f * visibleSize.height) / backButton->getContentSize().height
		);
	this->addChild(toPrevSceneButton);
}
