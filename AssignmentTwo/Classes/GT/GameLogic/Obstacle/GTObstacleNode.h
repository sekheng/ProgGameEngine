#ifndef GT_OBSTACLENODE_H
#define GT_OBSTACLENODE_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "../../../GT/Common/GTMacros.h"
#include "GTObstacleMacros.h"
#include "../../../GT/GameLogic/GTCollisionCategory.h"

// Include MK
#include "../../../MK/Common/MKMathsHelper.h"
#include "../../../MK/Graphics/MKSprite.h"
#include "../../../MK/SceneManagement/MKScene.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacleNode : public cocos2d::Node
{
    typedef cocos2d::Node Super;

private:
    MKScene* m_Scene = nullptr;
    mkBool m_IsPaused = false;

protected:
    EventListenerPhysicsContact* m_ContactListener = NULL;

    virtual bool OnContactBegin(cocos2d::PhysicsContact& _contact) = 0;

public:
    static const mkF32 m_DestroyedAnimationDuration;

    MKScene* GetScene() { return m_Scene; }
    const MKScene* GetScene() const { return m_Scene; }

    mkBool IsPaused() { return m_IsPaused; }

    const EventListenerPhysicsContact* GetContactListener() const { return m_ContactListener; }
    EventListenerPhysicsContact* GetContactListener() { return m_ContactListener; }

    virtual void PauseObstacle() { m_IsPaused = true; }
    virtual void ResumeObstacle() { m_IsPaused = false; }

    virtual void DestroyObstacle() { removeFromParent(); }

protected:
    GTObstacleNode(MKScene* _scene) : m_Scene(_scene) {}
    virtual ~GTObstacleNode() {}

    virtual bool init() override { return Super::init(); }
    virtual void update(gtF32 _deltaTime) override { Super::update(_deltaTime); };
};

const mkF32 GTObstacleNode::m_DestroyedAnimationDuration = 1.5f;

NS_GT_END

#endif // GT_OBSTACLENODE_H