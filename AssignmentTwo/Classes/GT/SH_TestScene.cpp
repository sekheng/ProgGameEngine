// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"
#include "../MK/Common/MKMacros.h"

// Include Assignment
#include "SH_TestScene.h"

SH_TestScene::SH_TestScene()
{
}

SH_TestScene::~SH_TestScene()
{
    
}

// on "init" you need to initialize your instance
bool SH_TestScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    // Trying to figure out the Lua stuff

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Input Testing
    InitializeInput();

    scheduleUpdate();

    return true;
}
void SH_TestScene::InitializeInput()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(SH_TestScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SH_TestScene::onKeyPressed(EventKeyboard::KeyCode key, Event*event)
{
    if (key == EventKeyboard::KeyCode::KEY_UP_ARROW)
    {
        MKSceneManager::GetInstance()->ReplaceScene("GameScene");
    }
}
void SH_TestScene::OnButton(EventCustom* _event)
{
    MKInputButton* buttonEvent = static_cast<MKInputButton*>(_event->getUserData());
    switch (buttonEvent->m_ButtonState)
    {
    case MinamiKotori::MKInputButton::ButtonState::PRESS:
        MKSceneManager::GetInstance()->ReplaceScene("GameScene");
        break;
    case MinamiKotori::MKInputButton::ButtonState::HOLD:
        break;
    case MinamiKotori::MKInputButton::ButtonState::RELEASE:
        break;
    default:
        break;
    }
}

void SH_TestScene::OnClick(EventCustom* _event)
{
}

void SH_TestScene::OnAxis(EventCustom* _event)
{
}

void SH_TestScene::update(float _deltaTime)
{
    m_SceneChangeCounter -= _deltaTime;
    if (m_SceneChangeCounter < 0.0f)
    {
        //MKSceneManager::GetInstance()->ReplaceScene("MKSceneDerived");
        //MKSceneManager::GetInstance()->PushScene("MKSceneDerived");
        m_SceneChangeCounter = 3.0f;
    }
}

void SH_TestScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
