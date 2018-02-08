#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "../MK/Common/MKMacros.h"
#include "../MK/Input/MKInputManager.h"
#include "../MK/Input/MKInput.h"
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/Graphics/MKSprite.h"

#include "../UIClass/UICreator.h"
#ifndef WIN32
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif
#endif

USING_NS_CC;
USING_NS_MK

class MainMenuScene : public MKScene, sdkbox::FacebookListener
{
    typedef MKScene Super;

protected:
    // Background
	MKSprite* m_Background = nullptr;
    void InitialiseBackground();

    // UI
    void InitialiseUI();

    // Facebook
	//TEMP VARIABLE//
	ui::Button* m_FacebookLoginButton;
	ui::Button* m_FacebookLogoutButton;
    void InitialiseFacebookUI();
    void ToggleFacebookUI(bool _isLoggedIn);

    // Input
    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event) {}
    virtual void OnAxis(EventCustom * _event) {}

public:
    // Constructor(s) & Destructor
    MainMenuScene() {}
    virtual ~MainMenuScene();
    CREATE_FUNC(MainMenuScene);

    virtual bool init();
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
};

#endif // __MENU_SCENE_H__
