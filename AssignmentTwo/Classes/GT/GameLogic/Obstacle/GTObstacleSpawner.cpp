// Include GT
#include "GTObstacleSpawner.h"

// Include Obstacles
#include "GTObstacle_Missile.h"
#include "GTObstacle_Spike.h"

// Include MK
#include "..\..\..\MK\Common\MKMathsHelper.h"

NS_GT_BEGIN

GTObstacleSpawner::GTObstacleSpawner(MKScene* _scene, cocos2d::Node* _playerNode, gtF32 _playerVelocityX, gtF32 _initialObstacleSpawnPositionX) :
    m_Scene(_scene),
    m_PlayerNode(_playerNode),
    m_PlayerVelocityX(_playerVelocityX),
    m_InitialSpawnPositionX(_initialObstacleSpawnPositionX),
    m_SpawnPositionX(_initialObstacleSpawnPositionX)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_MinObstacleInterval = visibleSize.height * 0.5f;
    m_MaxObstacleInterval = visibleSize.height * 1.5f;
}

GTObstacleSpawner::~GTObstacleSpawner()
{
    DespawnAllObstacles();
}

void GTObstacleSpawner::Reset()
{
    DespawnAllObstacles();
    m_SpawnPositionX = m_InitialSpawnPositionX;
}

gtF32 GTObstacleSpawner::CalculateObstacleInterval() const
{
    return Director::getInstance()->getVisibleSize().height * 0.5f;

    gtF32 denominatorSquareRoot = m_DifficultyMultiplier * (gtF32)m_ObstaclesSpawned;
    gtF32 result = 1.0f / (denominatorSquareRoot * denominatorSquareRoot) * m_MaxObstacleInterval;
    return MKMathsHelper::Clamp<gtF32>(result, m_MinObstacleCount, m_MaxObstacleInterval);
}

gtF32 GTObstacleSpawner::CalculateScreenRight() const
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return m_Scene->getDefaultCamera()->getPositionX() + visibleSize.width * 0.5f;
}

gtF32 GTObstacleSpawner::CalculateScreenLeft() const
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    return m_Scene->getDefaultCamera()->getPositionX() - visibleSize.width * 0.5f;
}

void GTObstacleSpawner::Update(gtF32 _deltaTime)
{
    // Spawn Obstacles
    if (m_ObstacleList.size() < m_MinObstacleCount)
    {
        SpawnObstacleBatch();
    }
    DespawnOutOfScreenObstacles();
}

void GTObstacleSpawner::DespawnAllObstacles()
{
    for (std::list<GTObstacleNode*>::iterator i = m_ObstacleList.begin(); i != m_ObstacleList.end(); ++i)
    {
        GTObstacleNode* obstacleNode = *i;
        obstacleNode->removeFromParent();
    }

    m_ObstacleList.clear();
}

// Despawn Obstacles when they are out of the screen.
void GTObstacleSpawner::DespawnOutOfScreenObstacles()
{
    for (std::list<GTObstacleNode*>::iterator i = m_ObstacleList.begin(); i != m_ObstacleList.end(); )
    {
        GTObstacleNode* obstacleNode = *i;
        gtF32 screenLeft = CalculateScreenLeft();
        if (obstacleNode->getPositionX() + (obstacleNode->getContentSize().width * obstacleNode->getScaleX()) < screenLeft)
        {
            obstacleNode->removeFromParent();
            i = m_ObstacleList.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

void GTObstacleSpawner::MoveAllObstacles(gtF32 _distance)
{
    for (std::list<GTObstacleNode*>::iterator i = m_ObstacleList.begin(); i != m_ObstacleList.end(); ++i)
    {
        GTObstacleNode* obstacleNode = *i;
        obstacleNode->setPosition(obstacleNode->getPosition() - Vec2(_distance, 0.0f));
    }

    m_SpawnPositionX -= _distance;
}

void GTObstacleSpawner::SpawnObstacleBatch()
{
    gtF32 playerPositionX = m_PlayerNode->getPositionX();
    CC_ASSERT(m_PlayerNode->getPositionX() <= m_SpawnPositionX);

    for (gtU32 i = 0; i < m_BatchSize; ++i)
    {
        gtF32 obstacleInterval = CalculateObstacleInterval();

        ObstacleType obstacleType = (ObstacleType)MKMathsHelper::RandomInt(0, (mkS32)NUM_OBSTACLETYPE);
        switch (obstacleType)
        {
        case GinTama::GTObstacleSpawner::MISSILE:
            SpawnMissile();
            break;
        case GinTama::GTObstacleSpawner::SPIKE:
            //SpawnSpike();
            SpawnMissile();
            break;
        default:
            CC_ASSERT(false);
            break;
        }

        ++m_ObstaclesSpawned;
        m_SpawnPositionX += obstacleInterval;
    }
}

void GTObstacleSpawner::SpawnSpike()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    GTObstacle_Spike* obstacle = GTObstacle_Spike::Create(m_Scene);
    obstacle->setPosition(m_SpawnPositionX, visibleSize.height * 0.1f);

    m_Scene->addChild(obstacle);
    m_ObstacleList.push_back(obstacle);
}

void GTObstacleSpawner::SpawnMissile()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    gtF32 playerPositionX = m_PlayerNode->getPositionX();

    // Even though we call it m_SpawnPositionX, it only applies to static obstacles.
    // For moving obstacles such as missiles, we need to spawn it further away,
    // such that when the player reaches the spawn position, so will the missile.
    gtF32 playerToSpawnPositionDistance = m_SpawnPositionX - playerPositionX;
    gtF32 secondsToSpawnPosition = playerToSpawnPositionDistance / m_PlayerVelocityX;

    // We need the missile to reach m_SpawnPositionX the same time as the player.
    gtF32 missileVelocityX = GTObstacle_Missile::GetHorizontalVelocity();
    gtF32 missileSpawnPositionX = MKMathsHelper::Abs<gtF32>(secondsToSpawnPosition * missileVelocityX) + m_SpawnPositionX;
    gtF32 missileSpawnPositionY = (gtF32)MKMathsHelper::RandomInt((gtS32)(visibleSize.height * 0.2f), (gtS32)(visibleSize.height * 0.8f));

    if (missileSpawnPositionY > visibleSize.height * 0.8f)
    {
        int test = 0;
    }

    GTObstacle_Missile* obstacle = GTObstacle_Missile::Create(m_Scene);
    obstacle->setPosition(missileSpawnPositionX, missileSpawnPositionY);

    m_Scene->addChild(obstacle);
    m_ObstacleList.push_back(obstacle);
}

NS_GT_END