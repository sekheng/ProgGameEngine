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
    typedef MKScene Super;

protected:
    mkF32 m_TimeCounter = 0.0f;

    void InitializeLoadingUI();

    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event) {}
    virtual void OnAxis(EventCustom * _event) {}

public:
    GameOverScene() {}
    virtual ~GameOverScene() {}
    CREATE_FUNC(GameOverScene);

	virtual bool init();
	virtual void update(float _deltaTime) override;
};

#endif // __GAME_OVER_SCENE_H__