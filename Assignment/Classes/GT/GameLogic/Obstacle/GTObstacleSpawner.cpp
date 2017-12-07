// Include GT
#include "GTObstacleSpawner.h"

// Include Obstacles
#include "GTObstacle_Missile.h"

// Include MK
#include "MK/Common/MKMathsHelper.h"

NS_GT_BEGIN

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
    
    SpawnMissileUpdate(_deltaTime);
}

void GTObstacleSpawner::SpawnMissileUpdate(gtF32 _deltaTime)
{
    if ((m_TimeToSpawnMissileTimer -= _deltaTime) < 0.0f)
    {
        m_TimeToSpawnMissileTimer = m_TimeToSpawnMissile;

        SpawnMissile();
    }
}

void GTObstacleSpawner::SpawnMissile()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

    GTObstacle_Missile* obstacle = GTObstacle_Missile::Create(m_Scene);
    gtF32 desiredObstacleScale = (visibleSize.height * 0.03f) / obstacle->getContentSize().height;
    obstacle->setScale(desiredObstacleScale, desiredObstacleScale);
    obstacle->setPosition(visibleOrigin.x + visibleSize.width, m_Player->getPosition().y);

    gtF32 obstacleSpeed = -visibleSize.height * 2.0f;
    auto obstacleAction = MoveBy::create(1.0f, Vec2(obstacleSpeed, 0.0f));
    obstacle->runAction(RepeatForever::create(obstacleAction));

    // Give the obstacle enough lifetime to travel through the screen twice.
    gtF32 obstacleLifeTime = NS_MK::MKMathsHelper::Abs<gtF32>(visibleSize.width / obstacleSpeed) * 2.0f;
    obstacle->SetLifetime(obstacleLifeTime);

    m_Scene->addChild(obstacle);
}

NS_GT_END