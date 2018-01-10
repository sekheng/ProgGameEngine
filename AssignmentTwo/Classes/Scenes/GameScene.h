#ifndef GAME_SCENE_H
#define GAME_SCENE_H

// Include MK
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/Graphics/MKSprite.h"

// Include GT
#include "../GT/Common/GTMacros.h"
#include "../GT/GameLogic/Obstacle/GTObstacleSpawner.h"
#include "../GT/GameLogic/PowerUp/GTPowerUpSpawner.h"
#include "../GT/GameLogic/GTCharacterStatNode.h"
#include "ui/UIButton.h"

USING_NS_CC;
USING_NS_MK
USING_NS_GT

class GameScene : public MKScene
{
	typedef MKScene Super;

private:
	enum BackgroundLayers
	{
		SKY,
		REAR,
		MIDDLE,
		FRONT,

		NUM_BACKGROUNDLAYERS,
	};

    // Environment
	MKSprite** m_Backgrounds = nullptr;
	MKSprite* m_Ground = nullptr;
    // Obstacles
    GTObstacleSpawner* m_ObstacleSpawner = nullptr;
	// PowerUps
	GTPowerUpSpawner* m_PowerUpSpawner = nullptr;
    // UI
    Label* m_HighScoreTxt = nullptr;
    std::vector<Node*> m_ArrayOfGameOverUI;
    // Player
    Node* m_PlayerNode;
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

    // Update
    void ScrollBackgrounds(float _deltaTime);
    void UpdateCamera();
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
};

#endif