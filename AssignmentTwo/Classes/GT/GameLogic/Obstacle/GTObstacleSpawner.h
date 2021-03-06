#ifndef GT_OBSTACLESPAWNER_H
#define GT_OBSTACLESPAWNER_H

// Include Cocos
#include "cocos2d.h"

// Include GT
#include "../../../GT/Common/GTMacros.h"
#include "GTObstacleNode.h"

// Include MK
#include "../../../MK/SceneManagement/MKScene.h"
#include "../../../MK/Graphics/MKSprite.h"
#include "../../../MK/Common/MKMathsHelper.h"

// Include STL
#include <list>
#include <functional>

USING_NS_CC;
USING_NS_MK
using namespace std;

NS_GT_BEGIN

class GTObstacleSpawner
{
public:
    enum ObstacleType
    {
        SPIKE = 0,
        MISSILE,
		SAW,
        LASER,

        NUM_OBSTACLETYPE,
    };

private:
    MKScene* m_Scene;
    cocos2d::Node* m_PlayerNode;
    gtF32 m_PlayerVelocityX;

    // This is the interval between each obstacle reaching the player.
    gtF32 CalculateObstacleInterval() const;
    gtF32 CalculateScreenRight() const;
    gtF32 CalculateScreenLeft() const;

    // Spawning
    std::list<GTObstacleNode*> m_ObstacleList;
    gtU32 m_MinObstacleCount = 16;
    gtU32 m_BatchSize = 8;
    gtU32 m_ObstaclesSpawned = 0;
    gtF32 m_SpawnPositionX;
    gtF32 m_InitialSpawnPositionX;

    // Difficulty
    gtF32 m_MinObstacleInterval;
    gtF32 m_MaxObstacleInterval;
    gtF32 m_DifficultyMultiplier = 0.1f;

    void SpawnObstacleBatch();

    void DespawnAllObstacles();
    void DespawnOutOfScreenObstacles();
    
    // Obstacle Spawning
    void SpawnSpike();
    void SpawnMissile();
	void SpawnSaw();
    void SpawnLaser();

public:
    // Constructor(s) & Destructor
    // The initial spawn position is to allow the obstacles some buffer time to spawn, since some obstacles take multiple seconds to get ready
    // before it can hurt the player. Such as missile and Laser Beam.
    GTObstacleSpawner(MKScene* _scene, cocos2d::Node* _playerNode, gtF32 _playerVelocityX, gtF32 _initialObstacleSpawnPositionX);
    virtual ~GTObstacleSpawner();

    virtual void Update(gtF32 _deltaTime);

    void PauseAllObstacles(); // Call this function when opening the pause scene.
    void ResumeAllObstacles();  // Call this function when closing the pause scene.
    void MoveAllObstacles(gtF32 _distance);
    void Reset();
    void DestroyObstacles(mkU32 _numObstacles);

};

NS_GT_END

#endif