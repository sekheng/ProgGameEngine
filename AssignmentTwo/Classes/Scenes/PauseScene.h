#pragma once
#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../MK/Common/MKMacros.h"
#include "../MK/Input/MKInputManager.h"
#include "../MK/Input/MKInput.h"
#include "../MK/SceneManagement/MKScene.h"

USING_NS_CC;
USING_NS_MK

class PauseScene : public MKScene
{
private:
	Label* m_InputLabel = nullptr;

	float m_SceneChangeCounter = 3.0f;

public:
	PauseScene();
	virtual ~PauseScene();

	virtual bool init();
	virtual void update(float _deltaTime) override;

	void InitializePauseMenuButtons();

	void OnButton(EventCustom * _event);
	void OnClick(EventCustom * _event);
	void OnAxis(EventCustom * _event);

	// implement the "static create()" method manually
	CREATE_FUNC(PauseScene);
	MK_INITIALISEINPUT(PauseScene);
	MK_DEINITIALISEINPUT(PauseScene);
};

#endif // __MENU_SCENE_H__#pragma once
