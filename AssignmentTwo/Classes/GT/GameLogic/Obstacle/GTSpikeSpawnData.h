#ifndef GT_SPIKESPAWNDATA_H
#define GT_SPIKESPAWNDATA_H

// Include GT
#include "..\..\..\GT\Common\GTMacros.h"

NS_GT_BEGIN

struct GTSpikeSpawnData
{
public:
    gtF32 m_TimeToSpawn;
    gtF32 m_SpawnHeight;
    gtBool m_UpsideDown;

    GTSpikeSpawnData(const GTSpikeSpawnData& _other) :
        m_TimeToSpawn(_other.m_TimeToSpawn),
        m_SpawnHeight(_other.m_SpawnHeight),
        m_UpsideDown(_other.m_UpsideDown)
    {}
    GTSpikeSpawnData(gtF32 _timeToSpawn, gtF32 _spawnHeight, gtBool _upsideDown) :
        m_TimeToSpawn(_timeToSpawn),
        m_SpawnHeight(_spawnHeight),
        m_UpsideDown(_upsideDown)
    {}
    ~GTSpikeSpawnData() {}

    GTSpikeSpawnData& operator=(const GTSpikeSpawnData& _other)
    {
        m_TimeToSpawn = _other.m_TimeToSpawn;
        m_SpawnHeight = _other.m_SpawnHeight;
        m_UpsideDown = _other.m_UpsideDown;
    }
};

NS_GT_END

#endif // GT_SPIKESPAWNDATA_H