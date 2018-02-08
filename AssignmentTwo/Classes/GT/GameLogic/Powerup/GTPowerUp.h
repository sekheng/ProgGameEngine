#ifndef GT_POWERUP_H
#define GT_POWERUP_H

#include "cocos2d.h"

#include "../../../GT/Common/GTMacros.h"
#include "../../../GT/GameLogic/GTGameplayMacros.h"
#include "../../../GT/GameLogic/GTCollisionCategory.h"

#include "../../../MK/Common/MKMathsHelper.h"
#include "../../../MK/Graphics/MKSprite.h"
#include "../../../MK/SceneManagement/MKScene.h"

USING_NS_MK

NS_GT_BEGIN

class GTPowerUp : public cocos2d::Node
{
	typedef cocos2d::Node Super;

private:
    MKScene *m_Scene = nullptr;
	mkBool m_IsPaused = false;

protected:
    EventListenerPhysicsContact* m_ContactListener = NULL;
	
	virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact) = 0;

public:

    MKScene *GetScene() { return m_Scene; }
    const MKScene* GetScene() const { return m_Scene; }

	mkBool IsPaused() { return m_IsPaused; }

    const EventListenerPhysicsContact* GetContactListener() const { return m_ContactListener; }
    EventListenerPhysicsContact* GetContactListener() { return m_ContactListener; }

	virtual void PausePowerUp() { m_IsPaused = true; }
	virtual void ResumePowerUp() { m_IsPaused = false; }

protected:
    GTPowerUp(MKScene *_scene) : m_Scene(_scene) {}
    virtual ~GTPowerUp() {}

	virtual bool init() { return cocos2d::Node::init(); }
	virtual void update(gtF32 _delta) {}
};

NS_GT_END

#endif 