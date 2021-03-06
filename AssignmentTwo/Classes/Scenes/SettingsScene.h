#ifndef __SETTINGS_SCENE_H__
#define __SETTINGS_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../MK/Common/MKMacros.h"
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/Graphics/MKSprite.h"

USING_NS_CC;
USING_NS_MK

class SettingsScene : public MKScene
{
    typedef MKScene Super;

protected:
    // Background
    MKSprite * m_Background = nullptr;
    void InitialiseBackground();

    // UI
    void InitialiseUI();

    // Input
    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event) {}
    virtual void OnAxis(EventCustom * _event) {}

public:
    SettingsScene() {}
    virtual ~SettingsScene() {}
    CREATE_FUNC(SettingsScene);
    
    virtual bool init();
};

#endif // __SETTINGS_SCENE_H__