#ifndef GT_OBSTACLENODE_H
#define GT_OBSTACLENODE_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "../Common/GTMacros.h"
#include "GTObstacleMacros.h"
#include "GTCollisionCategory.h"

// Include MK
#include "MK/Common/MKMathsHelper.h"
#include "MK/Graphics/MKSprite.h"

USING_NS_MK

NS_GT_BEGIN

class GTObstacleNode : public cocos2d::Node
{
    typedef cocos2d::Node Super;

private:
    float m_Lifetime = 5.0f;

protected:
    void UpdateLifetime(float _deltaTime)
    {
        if ((m_Lifetime -= _deltaTime) < 0.0f)
        {
            removeFromParent();
        }
    }

    virtual bool OnContactBegin(cocos2d::PhysicsContact& _contact) = 0;

public:
    void SetLifetime(float _lifeTime) { m_Lifetime = NS_MK::MKMathsHelper::Max<float>(_lifeTime, 0.0f); }
    float GetLifetime() const { return m_Lifetime; }

CC_CONSTRUCTOR_ACCESS:
    GTObstacleNode() {}
    virtual ~GTObstacleNode() {}

    virtual bool init() override { return Super::init(); }
    virtual void update(float _deltaTime) override { UpdateLifetime(_deltaTime); }

};

NS_GT_END

#endif // GT_OBSTACLENODE_H