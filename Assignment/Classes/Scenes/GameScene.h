#ifndef GAME_SCENE_H
#define GAME_SCENE_H

// Include MK
#include "MK/SceneManagement/MKScene.h"
//#include "MK/Gameplay/MKScrollableSprite.h"
#include "MK/Graphics/MKSprite.h"

USING_NS_CC;
USING_NS_MK

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

	void Deinitialise();

	// Input Callbacks
	MK_INITIALISEINPUT(GameScene);
	MK_DEINITIALISEINPUT(GameScene);
	virtual void OnButton(EventCustom * _event);
	virtual void OnClick(EventCustom * _event);
	virtual void OnAxis(EventCustom * _event);

    bool Chara_GroundContactBegin(PhysicsContact &_contact);
    bool CompareBitmasks(mkU32 _maskA, mkU32 _maskB);

public:
	GameScene() {}
	virtual ~GameScene() {}

    CREATEWITHPHYSICS_FUNC(GameScene);

    virtual bool initWithPhysics() override;
	virtual void update(float _deltaTime) override;

    Node *m_MainCharaNode;
};

#endif