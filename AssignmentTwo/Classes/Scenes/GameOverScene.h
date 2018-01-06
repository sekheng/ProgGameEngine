#pragma once
#ifndef __GAME_OVER_SCENE_H__
#define __GAME_OVER_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../MK/Common/MKMacros.h"
#include "../MK/Input/MKInputManager.h"
#include "../MK/Input/MKInput.h"
#include "../MK/SceneManagement/MKScene.h"

USING_NS_CC;
USING_NS_MK

class GameOverScene : public MKScene
{
private:
	Label* m_InputLabel = nullptr;

    float m_TimeCounter = 0.0f;

public:
	GameOverScene();
	virtual ~GameOverScene();

	virtual bool init();
	virtual void update(float _deltaTime) override;

	void InitializeLoadingUI();
	void InitializeGameOverMenuButtons();

	void OnButton(EventCustom * _event);
	void OnClick(EventCustom * _event);
	void OnAxis(EventCustom * _event);

	// implement the "static create()" method manually
	CREATE_FUNC(GameOverScene);
};

#endif // __MENU_SCENE_H__#pragma once