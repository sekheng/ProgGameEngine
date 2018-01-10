#ifndef GT_POWER_UP_SPAWNER_H
#define GT_POWER_UP_SPAWNER_H

#include "cocos2d.h"

#include "../../../GT/Common/GTMacros.h"
#include "GTPowerUp.h"
#include "../../../GT/GameLogic/Obstacle/GTObstacleSpawner.h"

#include "../../../MK/SceneManagement/MKScene.h"
#include "../../../MK/Graphics/MKSprite.h"
#include "../../../MK/Common/MKMathsHelper.h"

#include <list>
#include <functional>

USING_NS_CC;
USING_NS_MK
using namespace std;

NS_GT_BEGIN

class GTPowerUpSpawner
{
public:
	enum PowerUps
	{
		TIME_SLOW = 0,
		SPHERE_SHIELD,
		CLEAN_SLATE,

		MAX_POWERUPS,
	};

private:
	MKScene* m_Scene;
	GTObstacleSpawner* m_ObstacleSpawner;
	cocos2d::Node* m_PlayerNode;

	gtF32 CalculateScreenLeft() const;

	// Spawning
	std::list<GTPowerUp*> m_PowerUpsList;
	gtU32 m_MinPowerUpCount = 16;
	gtU32 m_BatchSize = 8;
	gtU32 m_PowerUpsSpawned = 0;
	gtF32 m_SpawnPositionX;
	gtF32 m_InitialSpawnPositionX;

	void SpawnPowerUpBatch();

	void DespawnAllPowerUps();
	void DespawnOutOfScreenPowerUps();

	// Obstacle Spawning
	void SpawnTimeSlow();
	void SpawnSphereShield();
	void SpawnCleanSlate();

public:
	// Constructor(s) & Destructor
	GTPowerUpSpawner(MKScene* _scene, cocos2d::Node* _playerNode, gtF32 _initialSpawnPositionX, GTObstacleSpawner* _obstacleSpawner);
	virtual ~GTPowerUpSpawner();

	virtual void Update(gtF32 _deltaTime);

	void PauseAllPowerUps(); // Call this function when opening the pause scene.
	void ResumeAllPowerUps();  // Call this function when closing the pause scene.
	void MoveAllPowerUps(gtF32 _distance);
	void Reset();

};

NS_GT_END

#endif

