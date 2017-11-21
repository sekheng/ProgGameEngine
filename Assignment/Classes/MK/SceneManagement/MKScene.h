#ifndef MK_TESTSCENE_H
#define MK_TESTSCENE_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMacros.h"
#include "MK/Input/MKInputManager.h"
#include "MK/Input/MKInput.h"
#include "MK/Common/MKPasskey.h"
#include "MKSceneMacros.h"

USING_NS_CC;

NS_MK_BEGIN

class MKSceneManager;

class MKScene : public cocos2d::Scene
{
	typedef cocos2d::Scene Super;

protected:
	// Input Event Listeners
	EventListenerCustom* m_ButtonListener = nullptr;
	EventListenerCustom* m_ClickListener = nullptr;
	EventListenerCustom* m_AxisListener = nullptr;

	// Input Callbacks
	virtual void OnButton(EventCustom * _event) = 0;
	virtual void OnClick(EventCustom * _event) = 0;
	virtual void OnAxis(EventCustom * _event) = 0;

public:
	MKScene() {}
	virtual ~MKScene() {}

	virtual bool init() { return Super::init(); }

	float GetAspectRatio() const
	{
		return Director::getInstance()->getVisibleSize().width / Director::getInstance()->getVisibleSize().height;
	}

	float GetAspectRatioInverse() const
	{
		return Director::getInstance()->getVisibleSize().height / Director::getInstance()->getVisibleSize().width;
	}
};

NS_MK_END

#endif