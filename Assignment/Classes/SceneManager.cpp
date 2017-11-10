#include "SceneManager.h"
#include "cocos2d.h"

NS_MK_BEGIN
SceneManager::SceneManager()
{
	m_currentScene = nullptr;
	m_nextScene = nullptr;
	m_prevScene = nullptr;
	m_backgroundScene = nullptr;
}

SceneManager::~SceneManager()
{
}

void SceneManager::AddScene(const std::string& _sceneName, cocos2d::Scene* _scene)
{
	if (CheckSceneExist(_sceneName))
	{
		throw std::exception("Duplicate Scene name provided");
	}

	if (_scene == nullptr)
	{
		throw std::invalid_argument("Invalid scene pointer");
	}

	m_sceneMap[_sceneName] = _scene;
}

void SceneManager::RemoveScene(const std::string& _sceneName)
{
	//Don't do anyth, scene doesnt exist
	if (!CheckSceneExist(_sceneName))
		return;

	cocos2d::Scene* target = m_sceneMap[_sceneName];
	if (target == m_currentScene)
	{
		throw std::exception("Unable to remove active scene");
	}

	delete target;
	m_sceneMap.erase(_sceneName);
}

void SceneManager::SetActiveScene(const std::string& _sceneName)
{
	if (!CheckSceneExist(_sceneName))
	{
		throw std::exception("Scene Does Not Exist");
	}

	if(m_currentScene == nullptr)
	{
		//set m_currentScene = m_sceneMap[_sceneName]
		m_currentScene = m_sceneMap[_sceneName];
		Director::getInstance()->runWithScene(m_currentScene);
	}
	else
	{
		//set m_nextScene = m_sceneMap[_sceneName];
		m_nextScene = m_sceneMap[_sceneName];
		m_currentScene = m_nextScene;
		Director::getInstance()->runWithScene(m_currentScene);
	}
}

bool SceneManager::CheckSceneExist(const std::string& _sceneName)
{
	return m_sceneMap.count(_sceneName) != 0;
}

void SceneManager::LoadSceneInBackground(const std::string& _sceneName)
{

}

NS_MK_END