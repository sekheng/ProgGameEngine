#ifndef START_SCREEN_SCENE_H
#define START_SCREEN_SCENE_H

// Include MK
#include "MK/SceneManagement/MKScene.h"
#include "MK/Graphics/MKSprite.h"

// Include STL
#include <cmath>

USING_NS_MK
USING_NS_CC;
using namespace std;

class StartScreenScene : public MKScene
{
    typedef MKScene Super;

private:
    MKSprite* m_SkyBackground = nullptr;
    MKSprite* m_Logo = nullptr;
    cocos2d::Label* m_StartLabel = nullptr;

    void InitialiseSkyBackground();
    void InitialiseLogo();
    void InitialiseStartLabel();

    // Input Callbacks
    MK_INITIALISEINPUT(StartScreenScene);
    MK_DEINITIALISEINPUT(StartScreenScene);
    virtual void OnButton(EventCustom * _event);
    virtual void OnClick(EventCustom * _event);
    virtual void OnAxis(EventCustom * _event);

    void Deinitialise();

public:
    StartScreenScene() {}
    virtual ~StartScreenScene() {}

    virtual bool init();
    virtual void update(float _deltaTime) override;

    CREATE_FUNC(StartScreenScene);
};

#endif // START_SCREEN_SCENE_H