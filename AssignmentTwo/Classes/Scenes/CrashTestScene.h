#ifndef CRASHTEST_SCENE_H
#define CRASHTEST_SCENE_H

// Include MK
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/Graphics/MKSprite.h"

USING_NS_CC;
USING_NS_MK

class CrashTestScene : public MKScene
{
	typedef MKScene Super;

private:
	void Deinitialise();

	float m_Timer = 1.0f;

	// Input Callbacks
	MK_INITIALISEINPUT(CrashTestScene);
	MK_DEINITIALISEINPUT(CrashTestScene);
	virtual void OnButton(EventCustom * _event);
	virtual void OnClick(EventCustom * _event);
	virtual void OnAxis(EventCustom * _event);

public:
	CrashTestScene() {}
	virtual ~CrashTestScene() {}

	CREATE_FUNC(CrashTestScene);

	virtual bool init() override;
	virtual void update(float _deltaTime) override;

};

#endif