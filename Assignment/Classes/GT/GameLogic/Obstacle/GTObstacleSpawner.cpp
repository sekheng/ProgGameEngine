// Include GT
#include "GTObstacleSpawner.h"
#include "GT/Actions/GTRemoveFromParent.h"
#include "GT/Actions/GTScaleUpAndDownAction.h"

// Include Obstacles
#include "GTObstacle_Missile.h"

// Include MK
#include "MK/Common/MKMathsHelper.h"

NS_GT_BEGIN

const mkString GTObstacleSpawner::m_MissileWarningSpriteFile = "Textures/Gameplay/Obstacle/Missile/Missile_Warning.png";

GTObstacleSpawner::GTObstacleSpawner(MKScene* _scene, cocos2d::Node* _player)
    : m_Scene(_scene), m_Player(_player)
{
}

GTObstacleSpawner::~GTObstacleSpawner()
{
}

void GTObstacleSpawner::Update(gtF32 _deltaTime)
{
    // Update TimePassed
    SetTimePassed(m_TimePassed + _deltaTime);

    // Temporary Test Spawning Code Start
    if ((m_TimeToSpawnMissileTimer -= _deltaTime) < 0.0f)
    {
        m_TimeToSpawnMissileTimer = m_TimeToSpawnMissile;
        AddMissileToSpawnQueue();
    }
    // Temporary Test Spawning Code End

    // Update Missiles
    UpdateMissiles(_deltaTime);
}

void GTObstacleSpawner::AddMissileToSpawnQueue()
{
    GTMissileSpawnData spawnData(1.5f, m_Player->getPositionY());
    SpawnMissileWarning(spawnData);
    m_MissileSpawnQueue.push(spawnData);
}

void GTObstacleSpawner::UpdateMissiles(gtF32 _deltaTime)
{
    if (m_MissileSpawnQueue.empty())
    {
        return;
    }

    GTMissileSpawnData* spawnData = &m_MissileSpawnQueue.front();
    if ((spawnData->m_TimeToSpawn -= _deltaTime) < 0.0f)
    {
        SpawnMissile(*spawnData);
        m_MissileSpawnQueue.pop();
    }
}

void GTObstacleSpawner::SpawnMissileWarning(const GTMissileSpawnData& _spawnData)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

    gtF32 desiredWarningSize = (visibleSize.height * 0.05f);
    MKSprite* warning = MKSprite::CreateWithSize(m_MissileWarningSpriteFile, Size(desiredWarningSize, desiredWarningSize), false);

    GTScaleUpAndDownAction* scaleUpAndDownAction = GTScaleUpAndDownAction::Create(_spawnData.m_TimeToSpawn, 0.8f, 1.2f, 0.0f, 10.0f);
    GTRemoveFromParentAction* removeFromParentAction = GTRemoveFromParentAction::Create();
    warning->runAction(Sequence::create(scaleUpAndDownAction, removeFromParentAction, NULL));
    warning->setPosition(visibleOrigin.x + visibleSize.width * 0.95f, _spawnData.m_SpawnHeight);
    m_Scene->GetUINode()->addChild(warning);
}

void GTObstacleSpawner::SpawnMissile(const GTMissileSpawnData& _spawnData)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

    GTObstacle_Missile* obstacle = GTObstacle_Missile::Create(m_Scene);
    gtF32 desiredObstacleScale = (visibleSize.height * 0.03f) / obstacle->getContentSize().height;
    obstacle->setScale(desiredObstacleScale, desiredObstacleScale);
    obstacle->setPosition(visibleOrigin.x + visibleSize.width, _spawnData.m_SpawnHeight);

    gtF32 obstacleSpeed = -visibleSize.height * 2.0f;
    auto obstacleAction = MoveBy::create(1.0f, Vec2(obstacleSpeed, 0.0f));
    obstacle->runAction(RepeatForever::create(obstacleAction));

    // Give the obstacle enough lifetime to travel through the screen twice.
    gtF32 obstacleLifeTime = NS_MK::MKMathsHelper::Abs<gtF32>(visibleSize.width / obstacleSpeed) * 2.0f;
    obstacle->SetLifetime(obstacleLifeTime);

    m_Scene->addChild(obstacle);
}

NS_GT_END