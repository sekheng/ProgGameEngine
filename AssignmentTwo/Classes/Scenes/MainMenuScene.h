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

USING_NS_CC;
USING_NS_MK

class MainMenuScene : public MKScene
{
private:
	Label* m_InputLabel = nullptr;
	MKSprite* m_SkyBackground = nullptr;

	float m_SceneChangeCounter = 3.0f;

	void InitialiseSkyBackground();

	void OnButton(EventCustom * _event);
	void OnClick(EventCustom * _event);
	void OnAxis(EventCustom * _event);

	MK_INITIALISEINPUT(MainMenuScene);
	MK_DEINITIALISEINPUT(MainMenuScene);

public:
	MainMenuScene();
	virtual ~MainMenuScene();

    virtual bool init();
    virtual void update(float _deltaTime) override;

    // a selector callback	
    void menuCloseCallback(cocos2d::Ref* pSender);

	CREATE_FUNC(MainMenuScene);

};

#endif // __MENU_SCENE_H__
