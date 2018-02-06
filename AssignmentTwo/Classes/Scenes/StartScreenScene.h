#ifndef START_SCREEN_SCENE_H
#define START_SCREEN_SCENE_H

// Include MK
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/Graphics/MKSprite.h"
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif
// Include STL
#include <cmath>

USING_NS_MK
USING_NS_CC;
using namespace std;

class StartScreenScene : public MKScene, sdkbox::FacebookListener
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
    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event);
    virtual void OnAxis(EventCustom * _event) {}

    void Deinitialise() { DeinitialiseInput(); }

public:
    StartScreenScene() {}
    virtual ~StartScreenScene() { Deinitialise(); }

    virtual bool init();
    virtual void update(float _deltaTime) override {}
    virtual void onEnter() override;
    virtual void onExit() override;

    virtual void onLogin(bool isLogin, const std::string& msg);
    virtual void onSharedSuccess(const std::string& message);
    virtual void onSharedFailed(const std::string& message);
    virtual void onSharedCancel();
    virtual void onAPI(const std::string& key, const std::string& jsonData);
    virtual void onPermission(bool isLogin, const std::string& msg);
    virtual void onFetchFriends(bool ok, const std::string& msg);
    virtual void onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends );
    virtual void onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg );
    virtual void onInviteFriendsResult( bool result, const std::string& msg );
    
    virtual void onGetUserInfo( const sdkbox::FBGraphUser& userInfo );
    
    CREATE_FUNC(StartScreenScene);
};

#endif // START_SCREEN_SCENE_H
