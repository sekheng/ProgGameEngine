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

MKScene* MKSceneManager::CreateScene(const mkString& _sceneName)
{
	std::unordered_map<mkString, std::function<MKScene*()> >::const_iterator iter = m_SceneMap.find(_sceneName);
	MK_ASSERTWITHMSG((iter != m_SceneMap.end()), "MKSceneManager::RemoveScene - Scene not found!");
	return iter->second();
}

void MKSceneManager::ReplaceScene(const mkString& _sceneName)
{
	MKScene* scene = CreateScene(_sceneName);
	MK_ASSERTWITHMSG((scene != nullptr), "MKSceneManager::ReplaceScene - Scene not found!");
    Director::getInstance()->replaceScene(scene);
}

void MKSceneManager::PushScene(const mkString& _sceneName)
{
	MKScene* scene = CreateScene(_sceneName);
	MK_ASSERTWITHMSG((scene != nullptr), "MKSceneManager::ReplaceScene - Scene not found!");
    MKScene* runningScene = (MKScene*)Director::getInstance()->getRunningScene();
	Director::getInstance()->pushScene(scene);
}

void MKSceneManager::PopScene()
{
	Director::getInstance()->popScene();
}

NS_MK_END