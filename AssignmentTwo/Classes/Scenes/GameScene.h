#ifndef GAME_SCENE_H
#define GAME_SCENE_H

// Include MK
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/Graphics/MKSprite.h"
#include "../MK/GameData/MKShopItem_Background.h"

// Include GT
#include "../GT/Common/GTMacros.h"
#include "../GT/GameLogic/Obstacle/GTObstacleSpawner.h"
#include "../GT/GameLogic/PowerUp/GTPowerUpSpawner.h"
#include "../GT/GameLogic/GTCharacterStatNode.h"
#include "ui/UIButton.h"
#ifndef WIN32
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#endif
#endif

USING_NS_CC;
USING_NS_MK
USING_NS_GT

class GameScene : public MKScene
#ifndef WIN32
#ifdef SDKBOX_ENABLED
, sdkbox::FacebookListener
#endif
#endif
{
	typedef MKScene Super;

private:
    mkString m_BackgroundFiles[MKShopItem_Background::BackgroundLayers::NUM_BACKGROUND_LAYERS];
	MKSprite** m_Backgrounds = nullptr;

    // Environment
	MKSprite* m_Ground = nullptr;

    // Obstacles
    GTObstacleSpawner* m_ObstacleSpawner = nullptr;

	// PowerUps
	GTPowerUpSpawner* m_PowerUpSpawner = nullptr;

    // UI
	Label* m_ScoreTxt = nullptr;
    Label* m_HighScoreTxt = nullptr;
	Label* m_SlowTimerTxt = nullptr;
    std::vector<Node*> m_ArrayOfGameOverUI;
    // Player
    Node* m_PlayerNode;
	// HighScore
	UserDefault *def;
	mkU32 m_HighScore;

    GTCharacterStatNode *m_CharaStatNode = nullptr;

    // Player Controls
    mkF32 m_SwipeDeadZone = 0.2f;
    // Slide
    mkBool m_SlideActionReleased = true;
    MKCursorPosition m_SlideClickStartPosition;
    MKCursorPosition m_SlideClickCurrentPosition;
    void SlideInput(const MKInputClick* _input);
    // Jump
    mkBool m_JumpActionReleased = true;
    MKCursorPosition m_JumpClickStartPosition;
    MKCursorPosition m_JumpClickCurrentPosition;
    void JumpInput(const MKInputClick* _input);

    // Initialisation
    void InitialisePlayer();
	void InitialiseGround();
	void InitialiseBackgrounds();
	void InitialiseUI();
    void InitialiseText();
    void InitialiseGameOverUI();
    void InitialiseObstacles();
	void InitialisePowerUps();
    void InitialiseCamera();

    // Update
    void ScrollBackgrounds(float _deltaTime);
    void UpdateUINode();
    void UpdateText();

    // Deinitialisation
    void Deinitialise();
    void ClearGameOverUI();
    void DeinitialiseObstacles();
	void DeinitialisePowerUps();

	// Input Callbacks
	MK_INITIALISEINPUT(GameScene);
	MK_DEINITIALISEINPUT(GameScene);
	virtual void OnButton(EventCustom * _event) override;
	virtual void OnClick(EventCustom * _event) override;
	virtual void OnAxis(EventCustom * _event) override;

public:
	GameScene() {}
    virtual ~GameScene() { Deinitialise(); }

    CREATEWITHPHYSICS_FUNC(GameScene);

    // Overrides
    virtual bool initWithPhysics() override;
	virtual void update(float _deltaTime) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void ShareHighScoreOnFB();
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

#endif
