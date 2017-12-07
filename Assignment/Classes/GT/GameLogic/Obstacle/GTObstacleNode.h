#ifndef GT_OBSTACLENODE_H
#define GT_OBSTACLENODE_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "GT/Common/GTMacros.h"
#include "GTObstacleMacros.h"
#include "GT/GameLogic/GTCollisionCategory.h"

// Include MK
#include "MK/Common/MKMathsHelper.h"
#include "MK/Graphics/MKSprite.h"
#include "MK/SceneManagement/MKScene.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacleNode : public cocos2d::Node
{
    typedef cocos2d::Node Super;

private:
    gtF32 m_Lifetime = 5.0f;
    MKScene* m_Scene = nullptr;

protected:
    EventListenerPhysicsContact* m_ContactListener = NULL;

    void UpdateLifetime(gtF32 _deltaTime)
    {
        if ((m_Lifetime -= _deltaTime) < 0.0f)
        {
            removeFromParent();
        }
    }

    virtual bool OnContactBegin(cocos2d::PhysicsContact& _contact) = 0;

public:
    void SetLifetime(gtF32 _lifeTime) { m_Lifetime = NS_MK::MKMathsHelper::Max<gtF32>(_lifeTime, 0.0f); }
    gtF32 GetLifetime() const { return m_Lifetime; }

    MKScene* GetScene() { return m_Scene; }
    const MKScene* GetScene() const { return m_Scene; }

    const EventListenerPhysicsContact* GetContactListener() const { return m_ContactListener; }
    EventListenerPhysicsContact* GetContactListener() { return m_ContactListener; }

CC_CONSTRUCTOR_ACCESS:
    GTObstacleNode(MKScene* _scene) : m_Scene(_scene) {}
    virtual ~GTObstacleNode() {}

    virtual bool init() override { return Super::init(); }
    virtual void update(gtF32 _deltaTime) override { UpdateLifetime(_deltaTime); }

};

NS_GT_END

#endif // GT_OBSTACLENODE_H