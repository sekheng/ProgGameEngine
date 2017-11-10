#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMacros.h"
#include "MK/Input/MKInputManager.h"
#include "MK/Input/MKInput.h"

USING_NS_CC;
USING_NS_MK

class GameScene : public cocos2d::Scene
{
private:
	EventListenerCustom* m_ButtonListener = nullptr;
	EventListenerCustom* m_ClickListener = nullptr;
	EventListenerCustom* m_AxisListener = nullptr;
	Label* m_InputLabel = nullptr;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float _deltaTime) override;

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void InitialiseInput();
	void OnButton(EventCustom * _event);
	void OnClick(EventCustom * _event);
	void OnAxis(EventCustom * _event);

	// Sek Heng here trying to do branching.

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};

#endif // __GameScene_SCENE_H__