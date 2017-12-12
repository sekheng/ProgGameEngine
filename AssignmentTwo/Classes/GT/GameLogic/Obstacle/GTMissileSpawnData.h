#ifndef GT_MISSILESPAWNDATA_H
#define GT_MISSILESPAWNDATA_H

// Include GT
#include "..\..\..\GT\Common\GTMacros.h"

NS_GT_BEGIN

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

NS_GT_END

#endif // GT_MISSILESPAWNDATA_H