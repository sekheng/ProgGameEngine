// Include STL
#include <thread>

// Include MK
#include "MKSceneManager.h"
#include "MKScene.h"

NS_MK_BEGIN

MKSceneManager::MKSceneManager()
{
}

MKSceneManager::~MKSceneManager()
{
}

MKScene* MKSceneManager::CreateScene(const std::string& _sceneName)
{
	std::unordered_map<std::string, std::function<MKScene*()> >::const_iterator iter = m_SceneMap.find(_sceneName);
	MK_ASSERTWITHMSG((iter != m_SceneMap.end()), "MKSceneManager::RemoveScene - Scene not found!");
	return iter->second();
}

void MKSceneManager::ReplaceScene(const std::string& _sceneName)
{
	MKScene* scene = CreateScene(_sceneName);
	MK_ASSERTWITHMSG((scene != nullptr), "MKSceneManager::ReplaceScene - Scene not found!");
	//Director::getInstance()->replaceScene(TransitionFade::create(m_SceneTransitionDuration, scene));
    Director::getInstance()->replaceScene(scene);
}

void MKSceneManager::PushScene(const std::string& _sceneName)
{
	MKScene* scene = CreateScene(_sceneName);
	MK_ASSERTWITHMSG((scene != nullptr), "MKSceneManager::ReplaceScene - Scene not found!");
	Director::getInstance()->pushScene(scene);
}

void MKSceneManager::PopScene()
{
	Director::getInstance()->popScene();
}

NS_MK_END