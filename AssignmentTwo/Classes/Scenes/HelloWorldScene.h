#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "..\MK\Common\MKMacros.h"
#include "..\MK\Input\MKInputManager.h"
#include "..\MK\Input\MKInput.h"
#include "..\MK\SceneManagement\MKScene.h"

USING_NS_CC;
USING_NS_MK

class HelloWorld : public MKScene
{
private:
	EventListenerCustom* m_ButtonListener = nullptr;
	EventListenerCustom* m_ClickListener = nullptr;
	EventListenerCustom* m_AxisListener = nullptr;
	Label* m_InputLabel = nullptr;
    Node *m_CharSpr;
	float m_SceneChangeCounter = 3.0f;
    int m_SoungIDCounter = 0;

    bool onContact(PhysicsContact &_contact);

public:
	HelloWorld();
	virtual ~HelloWorld();

    virtual bool init();
    virtual bool initWithPhysics();
    virtual void update(float _deltaTime) override;

    // a selector callback	
    void menuCloseCallback(cocos2d::Ref* pSender);

	void InitialiseInput();
    void OnButton(EventCustom * _event);
	void OnClick(EventCustom * _event);
	void OnAxis(EventCustom * _event);

    // Sek Heng here trying to do branching.

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    CREATEWITHPHYSICS_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
