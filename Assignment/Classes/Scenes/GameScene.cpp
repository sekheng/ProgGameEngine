// Include Game
#include "GameScene.h"

bool GameScene::init()
{
	if (!Super::init())
	{
		return false;
	}

	InitialiseBackgrounds();
	InitialiseGround();
	InitialiseInput();

	scheduleUpdate();

	return true;
}

void GameScene::InitialiseGround()
{
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Size desiredSize(128.0f, 128.0f);

	m_Ground = MKSprite::createWithSize("Environment/Ground.png", desiredSize, true);
	m_Ground->setAnchorPoint(Vec2(0.0f, 0.0f));
	m_Ground->setPosition(visibleOrigin.x, visibleOrigin.y);
	m_Ground->SetRepeat(8.0f, 1.0f);

	this->addChild(m_Ground);
}

void GameScene::InitialiseBackgrounds()
{
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// Set the backgrounds to null by default.
	//m_Backgrounds = new MKScrollableSprite*[NUM_BACKGROUNDLAYERS];
	//for (mkU32 i = 0; i < NUM_BACKGROUNDLAYERS; ++i) { m_Backgrounds[i] = nullptr; }

	//// Create the sprites.
	//m_Backgrounds[FRONT] = MKScrollableSprite::create("Environment/Backgrounds/Background_Front.png");

	//for (mkU32 i = 0; i < NUM_BACKGROUNDLAYERS; ++i)
	//{
	//	if (m_Backgrounds[i] != nullptr)
	//	{
	//		Rect textureRect = m_Backgrounds[i]->getTextureRect();
	//		textureRect.size.width = visibleSize.width;
	//		m_Backgrounds[i]->setAnchorPoint(Vec2(0.5f, 0.0f));
	//		m_Backgrounds[i]->setPosition(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y);
	//		this->addChild(m_Backgrounds[i]);
	//	}
	//}
}

void GameScene::OnButton(EventCustom * _event)
{
}

void GameScene::OnClick(EventCustom * _event)
{
}

void GameScene::OnAxis(EventCustom * _event)
{
}

void GameScene::update(float _deltaTime)
{
	m_Ground->OffsetTexture(0.1f * _deltaTime, 0.0f);

	//m_Backgrounds[FRONT]->Scroll(300.0f * _deltaTime, 0.0f);
}