#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../MK/Common/MKMacros.h"
#include "../MK/Input/MKInputManager.h"
#include "../MK/Input/MKInput.h"
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/Graphics/MKSprite.h"

#include "../UIClass/UICreator.h"

USING_NS_CC;
USING_NS_MK

class MainMenuScene : public MKScene
{
    typedef MKScene Super;

protected:
    // Background
	MKSprite* m_Background = nullptr;
    void InitialiseBackground();

    // UI
    void InitialiseUI();

    // Facebook
	//TEMP VARIABLE//
	ui::Button* m_FacebookLoginButton;
	ui::Button* m_FacebookLogoutButton;
    void InitialiseFacebookUI();

    // Input
    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event) {}
    virtual void OnAxis(EventCustom * _event) {}

public:
    // Constructor(s) & Destructor
    MainMenuScene() {}
    virtual ~MainMenuScene() {}
    CREATE_FUNC(MainMenuScene);

    virtual bool init();
};

#endif // __MENU_SCENE_H__
