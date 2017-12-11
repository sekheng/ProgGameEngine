#ifndef MK_TESTSCENE_H
#define MK_TESTSCENE_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "..\..\MK\Common\MKMacros.h"
#include "..\..\MK\Input\MKInputManager.h"
#include "..\..\MK\SceneManagement\MKSceneManager.h"
#include "..\..\MK\Input\MKInput.h"
#include "..\..\MK\Common\MKPasskey.h"
#include "MKSceneMacros.h"

USING_NS_CC;

NS_MK_BEGIN

class MKScene : public cocos2d::Scene
{
	typedef cocos2d::Scene Super;

protected:
    // UI Node
    cocos2d::Node* m_UINode = nullptr;

	// Input Event Listeners
	EventListenerCustom* m_ButtonListener = nullptr;
	EventListenerCustom* m_ClickListener = nullptr;
	EventListenerCustom* m_AxisListener = nullptr;

	// Input Callbacks
	virtual void OnButton(EventCustom * _event) = 0;
	virtual void OnClick(EventCustom * _event) = 0;
	virtual void OnAxis(EventCustom * _event) = 0;

    void QuitGame()
    {
        MKInputManager::GetInstance()->Destroy();
        MKSceneManager::GetInstance()->Destroy();
    }

public:
    static const mkS32 UIZOrder = 3000;

	MKScene() {}
	virtual ~MKScene() {}

	virtual bool init()
    {
        if (!Super::init()) { return false; }
        m_UINode = cocos2d::Node::create();
        m_UINode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_UINode->setPosition(Vec2::ZERO);
        addChild(m_UINode);
        return true;
    }
    
    virtual bool initWithPhysics()
    {
        if (!Super::initWithPhysics()) { return false; }
        m_UINode = cocos2d::Node::create();
        m_UINode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        m_UINode->setPosition(Vec2::ZERO);
        addChild(m_UINode);
        return true;
    }

    cocos2d::Node* GetUINode() { return m_UINode; }
    const cocos2d::Node* GetUINode() const { return m_UINode; }

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