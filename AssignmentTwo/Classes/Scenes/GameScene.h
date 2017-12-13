#ifndef GAME_SCENE_H
#define GAME_SCENE_H

// Include MK
#include "..\MK\SceneManagement\MKScene.h"
#include "..\MK\Graphics\MKSprite.h"

// Include GT
#include "..\GT\Common\GTMacros.h"
#include "..\GT\GameLogic\Obstacle\GTObstacleSpawner.h"

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

	MKSprite** m_Backgrounds = nullptr;
	MKSprite* m_Ground = nullptr;

	void InitialiseGround();
	void InitialiseBackgrounds();
	void ScrollBackgrounds(float _deltaTime);

    // Obstacles
    GTObstacleSpawner* m_ObstacleSpawner = nullptr;

    void InitialiseObstacles();
    void DeinitialiseObstacles();

	void Deinitialise();

	// Input Callbacks
	MK_INITIALISEINPUT(GameScene);
	MK_DEINITIALISEINPUT(GameScene);
	virtual void OnButton(EventCustom * _event) override;
	virtual void OnClick(EventCustom * _event) override;
	virtual void OnAxis(EventCustom * _event) override;

    // Camera
    void UpdateCamera();
    // UI
    void UpdateUINode();

public:
	GameScene() {}
	virtual ~GameScene() {}

    CREATEWITHPHYSICS_FUNC(GameScene);

    virtual bool initWithPhysics() override;
	virtual void update(float _deltaTime) override;

    Node *m_MainCharaNode;
};

#endif