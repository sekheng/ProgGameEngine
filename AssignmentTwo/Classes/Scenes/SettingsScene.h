#pragma once
#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../MK/Common/MKMacros.h"
#include "../MK/Input/MKInputManager.h"
#include "../MK/Input/MKInput.h"
#include "../MK/SceneManagement/MKScene.h"

USING_NS_CC;
USING_NS_MK

class SettingsScene : public MKScene
{
private:
	Label* m_InputLabel = nullptr;

	float m_SceneChangeCounter = 3.0f;

public:
	SettingsScene();
	virtual ~SettingsScene();

	virtual bool init();
	virtual void update(float _deltaTime) override;

	void OnButton(EventCustom * _event);
	void OnClick(EventCustom * _event);
	void OnAxis(EventCustom * _event);

	// Sek Heng here trying to do branching.

	// implement the "static create()" method manually
	CREATE_FUNC(SettingsScene);
    MK_INITIALISEINPUT(SettingsScene);
    MK_DEINITIALISEINPUT(SettingsScene);
};

#endif // __MENU_SCENE_H__