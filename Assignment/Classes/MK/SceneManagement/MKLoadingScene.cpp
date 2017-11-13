// Include MK
#include "MKLoadingScene.h"
#include "../Common/MKAssertions.h"
#include "MKSceneManager.h"

NS_MK_BEGIN

void MKLoadingScene::problemLoading(const char* _fileName)
{
	printf("Error while loading: %s\n", _fileName);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool MKLoadingScene::init()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_ProgressLabel = Label::createWithTTF("Loading: %", "fonts/Marker Felt.ttf", 24);
	if (m_ProgressLabel == nullptr)
	{
		problemLoading("'fonts/Marker Felt.ttf'");
		MK_ASSERT(false);
	}
	// position the label on the center of the screen
	m_ProgressLabel->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - m_ProgressLabel->getContentSize().height));
	// add the label as a child to this layer
	this->addChild(m_ProgressLabel, 1);

	m_ProgressListener = EventListenerCustom::create(MKSceneManager::GenerateSceneLoadProgressEventName(m_SceneToLoad), CC_CALLBACK_1(MKLoadingScene::ProgressUpdateCallback, this));
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(m_ProgressListener, 1);

	MKSceneManager::GetInstance()->LoadScene({}, m_SceneToLoad);
}

void MKLoadingScene::ProgressUpdateCallback(EventCustom* _event)
{
	MKSceneLoadProgress* progress = (MKSceneLoadProgress*)_event->getUserData();
	std::string labelString = "Loading:\n" + progress->GetMessage();
	m_ProgressLabel->setString(labelString.c_str());

	if (progress->IsCompleted())
	{
		MKSceneManager::GetInstance()->ReplaceScene(m_SceneToLoad);
	}
}

NS_MK_END