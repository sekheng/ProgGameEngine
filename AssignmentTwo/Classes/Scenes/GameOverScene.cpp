// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"

// Include Others
#include "GameOverScene.h"

bool GameOverScene::init()
{
	if (!Super::init()) { return false; }

    m_TimeCounter = 0;
	InitializeLoadingUI();
    scheduleUpdate();

	return true;
}

void GameOverScene::InitializeLoadingUI()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	mkF32 desiredObstacleScale = (visibleSize.height * 24.0f) / this->getContentSize().height;
	auto m_FinalHighScoreTxt = Label::createWithTTF("HighScore", "fonts/Marker_Felt.ttf", desiredObstacleScale);
	m_FinalHighScoreTxt->setTextColor(Color4B::WHITE);
	m_FinalHighScoreTxt->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_FinalHighScoreTxt->setPosition(visibleSize.width * 0.5f, visibleSize.height * 0.7f);
	m_FinalHighScoreTxt->setString("Loading");
	
	this->addChild(m_FinalHighScoreTxt);
}

void GameOverScene::update(float _deltaTime)
{
    m_TimeCounter += _deltaTime;
    if (m_TimeCounter > 0.2f)
    {
        MKSceneManager::GetInstance()->ReplaceScene("GameScene");
    }
}