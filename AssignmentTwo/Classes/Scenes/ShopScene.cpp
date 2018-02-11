// Include Cocos
#include "../GT/Audio/GTSimperMusicSys.h"

// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"
#include "../MK/Common/MKMacros.h"
#include "../MK/GameData/MKGameBackgroundData.h"
#include "../MK/GameData/MKGameDataLoader.h"
#include "../MK/GameData//MKPlayerData.h"

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
	playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
	playerData->LoadData(playerData->GetWritablePath());

	shopItem_Background = MKGameDataLoader::GetInstance()->GetGameData<MKGameBackgroundData>();
	shopItem_Background->LoadData(shopItem_Background->GetCachedPath());
	shopItem_list = shopItem_Background->GetBackgrounds();

	InitialiseBackground();
	InitialiseUI();
	InitialisePlayerCoinUI();


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

void ShopScene::InitialisePlayerCoinUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float halfWidth = visibleSize.width * 0.5f;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float estimatedHalfPoint = origin.x + halfWidth;

	// Timer Text For Slow Time
	gtF32 desiredTextScale = (visibleSize.height * 24.0f) / this->getContentSize().height;
	m_PlayerCoinsLabel = Label::createWithTTF("Player Coins: ", "fonts/Marker_Felt.ttf", desiredTextScale);
	m_PlayerCoinsLabel->setTextColor(Color4B::BLACK);
	m_PlayerCoinsLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_PlayerCoinsLabel->setPosition(origin.x + visibleSize.width * 0.25f, visibleSize.height * 0.9f);
	this->addChild(m_PlayerCoinsLabel);
}

void ShopScene::InitialiseShopItemUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float halfWidth = visibleSize.width * 0.5f;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float estimatedHalfPoint = origin.x + halfWidth;

	//Show Shop Item Name
	gtF32 desiredTextScale = (visibleSize.height * 24.0f) / this->getContentSize().height;
	m_ShopItemName = Label::createWithTTF("Name: None", "fonts/Marker_Felt.ttf", desiredTextScale);
	m_ShopItemName->setTextColor(Color4B::BLACK);
	m_ShopItemName->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_ShopItemName->setPosition(origin.x + visibleSize.width * 0.15f, visibleSize.height * 0.7f);
	m_ShopItemName->setVisible(true);
	this->addChild(m_ShopItemName);

	//Show Shop Item Price
	m_ShopItemPrice = Label::createWithTTF("Price: 0", "fonts/Marker_Felt.ttf", desiredTextScale);
	m_ShopItemPrice->setTextColor(Color4B::BLACK);
	m_ShopItemPrice->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_ShopItemPrice->setPosition(origin.x + visibleSize.width * 0.35f, visibleSize.height * 0.7f);
	m_ShopItemPrice->setVisible(true);
	this->addChild(m_ShopItemPrice);

	//Show Shop Item Sprites
	m_ItemDisplayNode = cocos2d::Node::create();
	m_ItemDisplayNode->setPosition(origin.x + visibleSize.width * 0.25f, visibleSize.height * 0.4f);
	this->addChild(m_ItemDisplayNode);
}

void ShopScene::InitialiseShopBackgrounds(MKShopItem_Background* _shopItem)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float halfWidth = visibleSize.width * 0.5f;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float estimatedHalfPoint = origin.x + halfWidth;

	// Background
	MKSprite* staticBackground = MKSprite::Create(_shopItem->GetBackgroundFile(MKShopItem_Background::STATIC));
	MKSprite* rearBackground = MKSprite::Create(_shopItem->GetBackgroundFile(MKShopItem_Background::REAR));
	MKSprite* middleBackground = MKSprite::Create(_shopItem->GetBackgroundFile(MKShopItem_Background::MIDDLE));
	MKSprite* frontBackground = MKSprite::Create(_shopItem->GetBackgroundFile(MKShopItem_Background::FRONT));

	staticBackground->setScale(0.3f * visibleSize.height / staticBackground->getContentSize().height);
	rearBackground->setScale(0.3f * visibleSize.height / rearBackground->getContentSize().height);
	middleBackground->setScale(0.3f * visibleSize.height / middleBackground->getContentSize().height);
	frontBackground->setScale(0.3f * visibleSize.height / frontBackground->getContentSize().height);

	// DO da resizig.
	m_ItemDisplayNode->removeAllChildren();
	m_ItemDisplayNode->addChild(staticBackground);
	m_ItemDisplayNode->addChild(rearBackground);
	m_ItemDisplayNode->addChild(middleBackground);
	m_ItemDisplayNode->addChild(frontBackground);
}

void ShopScene::UpdateButtonInfo(MKShopItem* _shopItem)
{
	m_ShopItemName->setString("Name: " + _shopItem->m_Name);
	m_ShopItemPrice->setString("Price: " + to_string(_shopItem->m_Cost));
}

void ShopScene::InitPurchaseButtons(MKShopItem_Background* _shopItem)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float halfWidth = visibleSize.width * 0.5f;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float estimatedHalfPoint = origin.x + halfWidth;

	ui::Button* button = ui::Button::create("ButtonNormal.png");
	mkF32 desiredScaleY = 0.15f * visibleSize.height / button->getContentSize().height;
	mkF32 desiredScaleX = 0.3f * visibleSize.width / button->getContentSize().width;

	
}

void ShopScene::InitialiseUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	float halfWidth = visibleSize.width * 0.5f;
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float estimatedHalfPoint = origin.x + halfWidth;

	Sprite *shopItemButtonSprite = Sprite::create("Textures/UI/City_button.png");
	ui::Button* button = ui::Button::create("ButtonNormal.png");
	mkF32 desiredScaleY = 0.15f * visibleSize.height / button->getContentSize().height;
	mkF32 desiredScaleX = 0.3f * visibleSize.width / button->getContentSize().width;
	mkF32 actualButtonHeight = desiredScaleY * button->getContentSize().height;
	mkF32 buttonPadding = actualButtonHeight * 0.1f;

	//Initialise shopScroller
	auto shopScroller = MKUICreator::GetInstance()->createScroller(
		ui::ScrollView::Direction::VERTICAL,
		Size(visibleSize.width * 0.5f, visibleSize.height * 0.8f),
		Size(visibleSize.width * 0.5f, shopItem_list.size() * button->getContentSize().height),
		true,
		Vec2(origin.x + visibleSize.width * 0.75f, visibleSize.height * 0.5f)
	);

	InitialiseShopItemUI();

	//Loop through the buttons and re-set position of each buttons
	for (int i = 0; i < shopItem_list.size(); ++i)
	{
		//Add Name of Shop Item and Price Labels here
		MKShopItem* shopItem = &shopItem_list[i];
		MKShopItem_Background* shopItemBackground = &shopItem_list[i];
		auto shopButton = MKUICreator::GetInstance()->createButton(
			Vec2(shopScroller->getContentSize().width * 0.5f, origin.y + shopScroller->getInnerContainerSize().height - ((mkF32)i * (actualButtonHeight + buttonPadding)) - (0.5f * actualButtonHeight)),
			"ButtonNormal.png",
			"ButtonSelected.png",
			shopItem_list[i].m_Name + "(" + std::to_string(shopItem_list[i].m_Cost) + ")",
			[=](Ref*) -> void
			{
				//SHOW THE ITEM ON THE LEFT
				this->UpdateButtonInfo(shopItem);
				this->InitialiseShopBackgrounds(shopItemBackground);
				if (!playerData->OwnsBackground(shopItem->m_Name))
				{
					m_buyButton = MKUICreator::GetInstance()->createButton(
						Vec2(origin.x + visibleSize.width * 0.25f, origin.y + visibleSize.height * 0.1f),
						"ButtonNormal.png",
						"ButtonSelected.png",
						"BUY",
						[=](Ref*) -> void
						{
							//Do Buying/Equipping here

							if (MKShopInterface::HasSufficientCoins(*shopItem))
							{
								MKShopInterface::PurchaseGameBackground(*shopItem, true);
								m_equipButton = MKUICreator::GetInstance()->createButton(
									Vec2(origin.x + visibleSize.width * 0.25f, origin.y + visibleSize.height * 0.1f),
									"ButtonNormal.png",
									"ButtonSelected.png",
									"EQUIP",
									[=](Ref*) -> void
									{
										//Equip Background
										MKShopInterface::EquipGameBackground(*shopItem, true);
										if (playerData->OwnsBackground(shopItem->m_Name) && playerData->GetEquippedBackground() == shopItem->m_Name)
										{
											m_equippedButton = MKUICreator::GetInstance()->createButton(
												Vec2(origin.x + visibleSize.width * 0.25f, origin.y + visibleSize.height * 0.1f),
												"ButtonNormal.png",
												"ButtonSelected.png",
												"EQUIPPED",
												[=](Ref*) -> void
												{
													//Equip Background
													MKShopInterface::EquipGameBackground(*shopItem, true);
												},
												15,
												desiredScaleX, desiredScaleY
												);
											this->addChild(m_equippedButton);
										}
									},
									15,
									desiredScaleX, desiredScaleY
									);
								this->addChild(m_equipButton);
							}
						},
						15,
						desiredScaleX, desiredScaleY
						);
					this->addChild(m_buyButton);
				}
				// Allow Player To Equip
				if (playerData->OwnsBackground(shopItem->m_Name))
				{
					m_equipButton = MKUICreator::GetInstance()->createButton(
						Vec2(origin.x + visibleSize.width * 0.25f, origin.y + visibleSize.height * 0.1f),
						"ButtonNormal.png",
						"ButtonSelected.png",
						"EQUIP",
						[=](Ref*) -> void
						{
							//Equip Background
							MKShopInterface::EquipGameBackground(*shopItem, true);
						},
						15,
						desiredScaleX, desiredScaleY
						);
					this->addChild(m_equipButton);
				}
			},	
			15,
			desiredScaleX, desiredScaleY
		);
		shopScroller->addChild(shopButton);
	}
	this->addChild(shopScroller);

	Sprite* backButton = Sprite::create("BackButton.png");

	auto toPrevSceneButton = MKUICreator::GetInstance()->createButton(
		Vec2(origin.x + backButton->getContentSize().width,origin.y + visibleSize.height - backButton->getContentSize().height),
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

void ShopScene::update(float _deltaTime)
{
	std::string PlayerCoins = "Coin: " + std::to_string(playerData->GetCoins());
	m_PlayerCoinsLabel->setString(PlayerCoins);
}
