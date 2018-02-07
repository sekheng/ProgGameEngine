#ifndef START_SCREEN_SCENE_H
#define START_SCREEN_SCENE_H

// Include MK
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/Graphics/MKSprite.h"

// Include STL
#include <cmath>

USING_NS_MK
USING_NS_CC;
using namespace std;

class StartScreenScene : public MKScene
{
    typedef MKScene Super;

protected:
    // Background
    MKSprite* m_Background = nullptr;
    void InitialiseBackground();

    // Logo
    MKSprite* m_Logo = nullptr;
    void InitialiseLogo();

    // Start Label
    cocos2d::Label* m_StartLabel = nullptr;
    void InitialiseStartLabel();

    // Input Callbacks
    MK_INITIALISEINPUT(StartScreenScene);
    MK_DEINITIALISEINPUT(StartScreenScene);
    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event);
    virtual void OnAxis(EventCustom * _event) {}

public:
    // Constructor(s) & Destructor
    StartScreenScene() {}
    virtual ~StartScreenScene() {}
    CREATE_FUNC(StartScreenScene);

    virtual bool init();
    virtual void update(float _deltaTime) override {}
    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif // START_SCREEN_SCENE_H