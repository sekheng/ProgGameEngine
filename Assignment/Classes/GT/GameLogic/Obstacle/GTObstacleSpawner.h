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

// Include STL
#include <queue>

USING_NS_CC;
USING_NS_MK
using namespace std;

NS_GT_BEGIN

class GTObstacleSpawner
{
private:
    // Resource Files
    static const mkString m_MissileWarningSpriteFile;
    static const mkString m_MissileWarningSoundName;

    struct GTMissileSpawnData
    {
    public:
        gtF32 m_TimeToSpawn;
        gtF32 m_SpawnHeight;

        GTMissileSpawnData(const GTMissileSpawnData& _other)
            : m_TimeToSpawn(_other.m_TimeToSpawn), m_SpawnHeight(_other.m_SpawnHeight)
        {}
        GTMissileSpawnData(gtF32 _timeToSpawn, gtF32 _spawnHeight)
            : m_TimeToSpawn(_timeToSpawn), m_SpawnHeight(_spawnHeight)
        {}
        ~GTMissileSpawnData() {}

        GTMissileSpawnData& operator=(const GTMissileSpawnData& _other)
        {
            m_TimeToSpawn = _other.m_TimeToSpawn;
            m_SpawnHeight = _other.m_SpawnHeight;
        }
    };

    MKScene* m_Scene = nullptr;
    cocos2d::Node* m_Player = nullptr;
    gtF32 m_TimePassed = 0.0f;

    // Temporary Experimentation Values Start
    gtF32 m_TimeToSpawnMissile = 3.0f;
    gtF32 m_TimeToSpawnMissileTimer = 0.0f;
    // Temporary Experimentation Values End

    // Missiles
    std::queue<GTMissileSpawnData> m_MissileSpawnQueue;

    void AddMissileToSpawnQueue();
    void UpdateMissiles(gtF32 _deltaTime);
    void SpawnMissileWarning(const GTMissileSpawnData& _spawnData);
    void SpawnMissile(const GTMissileSpawnData& _spawnData);

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