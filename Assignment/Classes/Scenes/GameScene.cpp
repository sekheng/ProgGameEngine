// Include Game
#include "GameScene.h"

// Include MK
#include "MK/SceneManagement/MKSceneManager.h"

bool GameScene::init()
{
	if (!Super::init())
	{
		return false;
	}

	InitialiseBackgrounds();
	InitialiseGround();

	scheduleUpdate();

    InitialiseInput();

    MKInputManager::GetInstance()->SetCurrentContext(MK_CONTEXT1);

	return true;
}

void GameScene::InitialiseGround()
{
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_Ground = MKSprite::create("Environment/Ground.png", true);
	m_Ground->setAnchorPoint(Vec2(0.0f, 0.0f));
	m_Ground->setPosition(visibleOrigin.x, visibleOrigin.y);

	float desiredHeight = visibleSize.height * 0.1f;
	float groundHeight = m_Ground->getContentSize().height;
	float desiredScale = desiredHeight / groundHeight;
	m_Ground->setScale(desiredScale, desiredScale);

	float groundWidth = m_Ground->getContentSize().width;	
	float numRepeat = visibleSize.width / (groundWidth * desiredScale);
	m_Ground->SetRepeat(numRepeat, 1.0f);

	this->addChild(m_Ground);
}

void GameScene::InitialiseBackgrounds()
{
	Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	m_Backgrounds = new MKSprite*[NUM_BACKGROUNDLAYERS];
	for (unsigned int i = 0; i < NUM_BACKGROUNDLAYERS; ++i) { m_Backgrounds[i] = nullptr; }

	m_Backgrounds[SKY] = MKSprite::create("Environment/Backgrounds/Background_Sky.png", true);
	m_Backgrounds[REAR] = MKSprite::create("Environment/Backgrounds/Background_Rear.png", true);	
	m_Backgrounds[MIDDLE] = MKSprite::create("Environment/Backgrounds/Background_Middle.png", true);
	m_Backgrounds[FRONT] = MKSprite::create("Environment/Backgrounds/Background_Front.png", true);	

	for (unsigned int i = 0; i < NUM_BACKGROUNDLAYERS; ++i)
	{
		if (m_Backgrounds[i] != nullptr)
		{
			m_Backgrounds[i]->setAnchorPoint(Vec2(0.0f, 0.0f));
			m_Backgrounds[i]->setPosition(visibleOrigin.x, visibleOrigin.y);

			float desiredHeight = visibleSize.height;
			float backgroundHeight = m_Backgrounds[i]->getContentSize().height;
			float desiredScale = desiredHeight / backgroundHeight;
			m_Backgrounds[i]->setScale(desiredScale, desiredScale);
			
			float backgroundWidth = m_Backgrounds[i]->getContentSize().width;
			float numRepeat = visibleSize.width / (backgroundWidth * desiredScale);
			m_Backgrounds[i]->SetRepeat(numRepeat, 1.0f);

			addChild(m_Backgrounds[i]);
		}
	}
}

void GameScene::OnButton(EventCustom * _event)
{
    /*MKInputButton* input = (MKInputButton*)_event->getUserData();
    switch (input->m_ButtonState)
    {
    case MKInputButton::ButtonState::HOLD:
        CCLOG("GameScene Button Hold");
        break;
    case MKInputButton::ButtonState::RELEASE:
        CCLOG("GameScene Button Release");
        break;
    case MKInputButton::ButtonState::PRESS:
        CCLOG("GameScene Button Press");
        break;
    }*/

    Deinitialise();
    MKSceneManager::GetInstance()->ReplaceScene("CrashTestScene");
}

void GameScene::OnClick(EventCustom * _event)
{
	MKInputClick* input = (MKInputClick*)_event->getUserData();

	if (input->m_ButtonState == MKInputButton::RELEASE)
	{
		Deinitialise();
		MKSceneManager::GetInstance()->ReplaceScene("CrashTestScene");
	}
}

void GameScene::OnAxis(EventCustom * _event)
{
}

void GameScene::ScrollBackgrounds(float _deltaTime)
{
	m_Ground->OffsetTexture(_deltaTime * 3.0f, 0.0f);
	
	m_Backgrounds[REAR]->OffsetTexture(_deltaTime * 0.05f, 0.0f);
	m_Backgrounds[MIDDLE]->OffsetTexture(_deltaTime * 0.075f, 0.0f);
	m_Backgrounds[FRONT]->OffsetTexture(_deltaTime * 0.1, 0.0f);
}

void GameScene::Deinitialise()
{
	DeinitialiseInput();
}

void GameScene::update(float _deltaTime)
{
	ScrollBackgrounds(_deltaTime);

    //Deinitialise();
    //QuitGame();
    //Director::getInstance()->end();
    //MKSceneManager::GetInstance()->ReplaceScene("CrashTestScene");
}