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

private:
	MKSprite* m_SkyBackground = nullptr;

	//TEMP VARIABLE//
	bool isLoggedin;
	ui::Button* FacebookLoginButton;
	ui::Button* FacebookLogoutButton;

	void InitialiseSkyBackground();

    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event) {}
    virtual void OnAxis(EventCustom * _event) {}
public:
    MainMenuScene() {}
    virtual ~MainMenuScene() {}

    virtual bool init();
	virtual void update(float _deltaTime) override;

    // a selector callback	
    void menuCloseCallback(cocos2d::Ref* pSender);

	void FacebookUI();

	CREATE_FUNC(MainMenuScene);

};

#endif // __MENU_SCENE_H__
