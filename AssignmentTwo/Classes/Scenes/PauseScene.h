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
    typedef MKScene Super;

protected:
    // Input
    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event) {}
    virtual void OnAxis(EventCustom * _event) {}

    // Initialisation
    void InitializeTitle();
    void InitializePauseMenuButtons();

public:
    PauseScene() {}
    virtual ~PauseScene() {}

	virtual bool init();

	// implement the "static create()" method manually
    CREATE_FUNC(PauseScene);
};

#endif // __PAUSE_SCENE_H__