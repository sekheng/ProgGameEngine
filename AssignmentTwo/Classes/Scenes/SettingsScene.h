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
    typedef MKScene Super;

public:
    SettingsScene() {}
    virtual ~SettingsScene() {}

	MKSprite* m_SkyBackground = nullptr;

	virtual bool init();

	void InitialiseSkyBackground();

    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event) {}
    virtual void OnAxis(EventCustom * _event) {}

	// implement the "static create()" method manually
	CREATE_FUNC(SettingsScene);
};

#endif // __SETTINGS_SCENE_H__