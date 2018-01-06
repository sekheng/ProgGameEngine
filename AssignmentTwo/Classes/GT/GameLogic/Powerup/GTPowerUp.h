#ifndef GT_POWERUP_H
#define GT_POWERUP_H

#include "cocos2d.h"
#include "../../../GT/Common/GTMacros.h"
#include "../../../GT/GameLogic/GTCollisionCategory.h"
#include "../../../MK/SceneManagement/MKScene.h"

NS_GT_BEGIN

class GTPowerUp : public cocos2d::Node
{
private:
    MKScene *m_Scene = nullptr;

protected:
    EventListenerPhysicsContact* m_ContactListener = NULL;

public:
    virtual void update(gtF32 _delta) {}
    virtual bool init() { return cocos2d::Node::init(); }

    MKScene *GetScene() { return m_Scene; }
    const MKScene* GetScene() const { return m_Scene; }

    const EventListenerPhysicsContact* GetContactListener() const { return m_ContactListener; }
    EventListenerPhysicsContact* GetContactListener() { return m_ContactListener; }

protected:
    virtual gtBool OnContactBegin(cocos2d::PhysicsContact& _contact) = 0;

    GTPowerUp(MKScene *_scene) : m_Scene(_scene) {}
    virtual ~GTPowerUp() {}
};

NS_GT_END

#endif 