#ifndef GAME_SCENE_H
#define GAME_SCENE_H

// Include MK
#include "MK/SceneManagement/MKScene.h"
#include "MK/Gameplay/MKScrollableSprite.h"
#include "MK/Graphics/MKSprite.h"

USING_NS_CC;
USING_NS_MK

class GameScene : public MKScene
{
	typedef MKScene Super;

private:
	enum BackgroundLayers
	{
		REAR,
		MIDDLE,
		FRONT,

		NUM_BACKGROUNDLAYERS,
	};

	MKScrollableSprite** m_Backgrounds = nullptr;
	MKSprite* m_Ground = nullptr;

	void InitialiseGround();
	void InitialiseBackgrounds();

	// Input Callbacks
	virtual void OnButton(EventCustom * _event);
	virtual void OnClick(EventCustom * _event);
	virtual void OnAxis(EventCustom * _event);
	MK_INITIALISEINPUT(GameScene);

public:
	CREATE_FUNC(GameScene);

	virtual bool init() override;
	virtual void update(float _deltaTime) override;

};

#endif