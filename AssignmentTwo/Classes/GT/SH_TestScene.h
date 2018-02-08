#pragma once

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../MK/Common/MKMacros.h"
#include "../MK/Input/MKInputManager.h"
#include "../MK/Input/MKInput.h"
#include "../MK/SceneManagement/MKScene.h"

USING_NS_CC;
USING_NS_MK

class SH_TestScene : public MKScene
{
private:
    EventListenerCustom* m_ButtonListener = nullptr;
    EventListenerCustom* m_ClickListener = nullptr;
    EventListenerCustom* m_AxisListener = nullptr;
    Label* m_InputLabel = nullptr;

    float m_SceneChangeCounter = 3.0f;

public:
    SH_TestScene();
    virtual ~SH_TestScene();

    virtual bool init();
    virtual void update(float _deltaTime) override;

    // a selector callback	
    void menuCloseCallback(cocos2d::Ref* pSender);

    void InitializeInput();
    void OnButton(EventCustom * _event);
    void OnClick(EventCustom * _event);
    void OnAxis(EventCustom * _event);

    void onKeyPressed(EventKeyboard::KeyCode key, Event*event);

    // implement the "static create()" method manually
    CREATE_FUNC(SH_TestScene);
    //MK_INITIALISEINPUT(SH_TestScene);
};
