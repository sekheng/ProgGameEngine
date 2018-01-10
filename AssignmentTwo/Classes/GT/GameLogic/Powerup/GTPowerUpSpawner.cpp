// Include GTPowerUpSpawner class
#include "GTPowerUpSpawner.h"

//Include PowerUps
#include "GTSlowTimePowerUp.h"
#include "GTSphereShieldPowerUp.h"
#include "GTCleanSlatePowerUp.h"

#include "../../../MK/Common/MKMathsHelper.h"

NS_GT_BEGIN

GTPowerUpSpawner::GTPowerUpSpawner(MKScene* _scene, cocos2d::Node* _PlayerNode, float _initialSpawnPositionX, GTObstacleSpawner* _obstacleSpawner)
: m_Scene(_scene),
  m_PlayerNode(_PlayerNode),
  m_InitialSpawnPositionX(_initialSpawnPositionX),
  m_SpawnPositionX(_initialSpawnPositionX),
  m_ObstacleSpawner(_obstacleSpawner)
{
}

GTPowerUpSpawner::~GTPowerUpSpawner()
{
	DespawnAllPowerUps();
}

void GTPowerUpSpawner::Reset()
{
	DespawnAllPowerUps();
	m_SpawnPositionX = m_InitialSpawnPositionX;
}

gtF32 GTPowerUpSpawner::CalculateScreenLeft() const
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	return m_Scene->getDefaultCamera()->getPositionX() - visibleSize.width * 0.5f;
}

void GTPowerUpSpawner::Update(gtF32 _deltaTime)
{
	if (m_PowerUpsList.size() < m_MinPowerUpCount)
	{
		SpawnPowerUpBatch();
	}
	DespawnOutOfScreenPowerUps();
}

void GTPowerUpSpawner::DespawnAllPowerUps()
{
	for (std::list<GTPowerUp*>::iterator i = m_PowerUpsList.begin(); i != m_PowerUpsList.end(); ++i)
	{
		GTPowerUp* powerUp = *i;
		powerUp->removeFromParent();
	}
	m_PowerUpsList.clear();
}

void GTPowerUpSpawner::DespawnOutOfScreenPowerUps()
{
	for (std::list<GTPowerUp*>::iterator i = m_PowerUpsList.begin(); i != m_PowerUpsList.end(); )
	{
		GTPowerUp* powerUp = *i;
		gtF32 screenLeft = CalculateScreenLeft();
		if (powerUp->getPositionX() + (powerUp->getContentSize().width * powerUp->getScaleX()) < screenLeft)
		{
			powerUp->removeFromParent();
			i = m_PowerUpsList.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void GTPowerUpSpawner::PauseAllPowerUps()
{
	for (std::list<GTPowerUp*>::iterator i = m_PowerUpsList.begin(); i != m_PowerUpsList.end(); ++i)
	{
		GTPowerUp* powerUp = *i;
		if (!powerUp->IsPaused())
		{
			powerUp->PausePowerUp();
		}
	}
}

void GTPowerUpSpawner::ResumeAllPowerUps()
{
	for (std::list<GTPowerUp*>::iterator i = m_PowerUpsList.begin(); i != m_PowerUpsList.end(); ++i)
	{
		GTPowerUp* powerUp = *i;
		if (powerUp->IsPaused())
		{
			powerUp->ResumePowerUp();
		}
	}
}

void GTPowerUpSpawner::MoveAllPowerUps(gtF32 _distance)
{
	for (std::list<GTPowerUp*>::iterator i = m_PowerUpsList.begin(); i != m_PowerUpsList.end(); ++i)
	{
		GTPowerUp* powerUp = *i;
		powerUp->setPosition(powerUp->getPosition() + Vec2(_distance, 0.0f));
	}

	m_SpawnPositionX += _distance;
}

void GTPowerUpSpawner::SpawnPowerUpBatch()
{
	gtF32 playerPositionX = m_PlayerNode->getPositionX();
	CC_ASSERT(m_PlayerNode->getPositionX() <= m_SpawnPositionX);
	PowerUps powerUpType;

	for (gtU32 i = 0; i < m_BatchSize; ++i)
	{
		gtF32 PowerUpInterval = MKMathsHelper::RandomInt(1000.0f, 6000.0f);

		gtU32 randomNumber = MKMathsHelper::RandomInt(0, 9);
		if(randomNumber >= 0 && randomNumber <= 4)
			powerUpType = GinTama::GTPowerUpSpawner::TIME_SLOW;
		if (randomNumber >= 5 && randomNumber <= 7)
			powerUpType = GinTama::GTPowerUpSpawner::SPHERE_SHIELD;
		if (randomNumber >= 8 && randomNumber <= 9)
			powerUpType = GinTama::GTPowerUpSpawner::CLEAN_SLATE;

		switch (powerUpType)
		{
		case GinTama::GTPowerUpSpawner::TIME_SLOW:
			SpawnTimeSlow();
			break;
		case GinTama::GTPowerUpSpawner::SPHERE_SHIELD:
			SpawnSphereShield();
			break;
		case GinTama::GTPowerUpSpawner::CLEAN_SLATE:
			SpawnCleanSlate();
			break;
		default:
			CC_ASSERT(false);
			break;
		}

		++m_PowerUpsSpawned;
		m_SpawnPositionX += PowerUpInterval;
	}
}

void GTPowerUpSpawner::SpawnTimeSlow()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	GTSlowTimePowerUp* powerUp = GTSlowTimePowerUp::Create(m_Scene);
	powerUp->setPosition(m_SpawnPositionX, MKMathsHelper::RandomInt(visibleSize.height * 0.1f, visibleSize.height - powerUp->getContentSize().height));

	m_Scene->addChild(powerUp);
	m_PowerUpsList.push_back(powerUp);
}

void GTPowerUpSpawner::SpawnSphereShield()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	GTSphereShieldPowerUp* powerUp = GTSphereShieldPowerUp::Create(m_Scene);
	powerUp->setPosition(m_SpawnPositionX, MKMathsHelper::RandomInt(visibleSize.height * 0.1f, visibleSize.height - powerUp->getContentSize().height));

	m_Scene->addChild(powerUp);
	m_PowerUpsList.push_back(powerUp);
}

void GTPowerUpSpawner::SpawnCleanSlate()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	GTCleanSlatePowerUp* powerUp = GTCleanSlatePowerUp::Create(m_Scene, m_ObstacleSpawner);
	powerUp->setPosition(m_SpawnPositionX, MKMathsHelper::RandomInt(visibleSize.height * 0.1f, visibleSize.height - powerUp->getContentSize().height));

	m_Scene->addChild(powerUp);
	m_PowerUpsList.push_back(powerUp);
}

NS_GT_END