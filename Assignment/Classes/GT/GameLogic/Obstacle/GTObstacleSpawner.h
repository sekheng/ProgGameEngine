#ifndef GT_OBSTACLESPAWNER_H
#define GT_OBSTACLESPAWNER_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "GT/Common/GTMacros.h"

// Include MK
#include "MK/SceneManagement/MKScene.h"
#include "MK/Graphics/MKSprite.h"
#include "MK/Common/MKMathsHelper.h"

USING_NS_CC;
USING_NS_MK

NS_GT_BEGIN

class GTObstacleSpawner
{
private:
    MKScene* m_Scene = nullptr;
    cocos2d::Node* m_Player = nullptr;
    gtF32 m_TimePassed = 0.0f;

    // Temporary Experimentation Values
    gtF32 m_TimeToSpawnMissile = 3.0f;
    gtF32 m_TimeToSpawnMissileTimer = 0.0f;

    void SpawnMissileUpdate(gtF32 _deltaTime);
    void SpawnMissile();

public:
    // Constructor(s) & Destructor
    GTObstacleSpawner(MKScene* _scene, cocos2d::Node* _player);
    virtual ~GTObstacleSpawner();

    virtual void Update(gtF32 _deltaTime);

    void SetTimePassed(gtF32 _timePassed) { m_TimePassed = MKMathsHelper::Max<gtF32>(_timePassed, 0.0f); }
    gtF32 GetTimePassed() const { return m_TimePassed; }

};

NS_GT_END

#endif