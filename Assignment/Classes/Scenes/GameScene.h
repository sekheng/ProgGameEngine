#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "Common/MKMacros.h"
#include "Input/MKInputManager.h"
#include "Input/MKInput.h"

USING_NS_CC;
USING_NS_MK;

class GameScene : public cocos2d::Scene
{
private:
	EventListenerCustom* m_ButtonListener = nullptr;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float _deltaTime) override;

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);
	void OnButtonInput(EventCustom * _event);

	void toGameScene();

	void startGame();

	// Sek Heng here trying to do branching.

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};

#endif // __HELLOWORLD_SCENE_H__
