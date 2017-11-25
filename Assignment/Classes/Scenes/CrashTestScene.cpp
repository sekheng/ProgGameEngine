#include "CrashTestScene.h"

// Include MK
#include "MK/SceneManagement/MKSceneManager.h"

bool CrashTestScene::init()
{
	if (!Super::init())
	{
		return false;
	}

	scheduleUpdate();

    InitialiseInput();

    MKInputManager::GetInstance()->SetCurrentContext(MK_CONTEXT2);

	return true;
}

void CrashTestScene::OnButton(EventCustom * _event)
{
    MKInputButton* input = (MKInputButton*)_event->getUserData();
    switch (input->m_ButtonState)
    {
    case MKInputButton::ButtonState::HOLD:
        CCLOG("CrashTestScene Button Hold");
        break;
    case MKInputButton::ButtonState::RELEASE:
        CCLOG("CrashTestScene Button Release");
        break;
    case MKInputButton::ButtonState::PRESS:
        CCLOG("CrashTestScene Button Press");
        break;
    }

	Deinitialise();
	MKSceneManager::GetInstance()->ReplaceScene("GameScene");
}

void CrashTestScene::OnClick(EventCustom * _event)
{
	MKInputClick* input = (MKInputClick*)_event->getUserData();

	if (input->m_ButtonState == MKInputButton::RELEASE)
	{
		Deinitialise();
		MKSceneManager::GetInstance()->ReplaceScene("GameScene");
	}
}

void CrashTestScene::OnAxis(EventCustom * _event)
{
}

void CrashTestScene::Deinitialise()
{
	DeinitialiseInput();
}

void CrashTestScene::update(float _deltaTime)
{
    //Deinitialise();
    //MKSceneManager::GetInstance()->ReplaceScene("GameScene");
}