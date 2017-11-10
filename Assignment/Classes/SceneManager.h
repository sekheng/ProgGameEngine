#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

//Include MKMacros
#include "MK\Common\MKAssertions.h"
#include "MK\Common\MKMacros.h"
#include "MK\Common\MKSingletonTemplate.h"

//Include Cocos
#include "cocos2d.h"

//Include STD
#include <map>
#include <string>

USING_NS_CC;

NS_MK_BEGIN

class SceneManager : public MKSingletonTemplate<SceneManager>
{
	friend MKSingletonTemplate<SceneManager>;
private:
	SceneManager();
	~SceneManager();

	int m_sceneCount;

	cocos2d::Scene* m_currentScene;
	cocos2d::Scene* m_nextScene;
	cocos2d::Scene* m_prevScene;
	cocos2d::Scene* m_backgroundScene;

	std::map<std::string, cocos2d::Scene*> m_sceneMap;
public:
	template<typename T>
	T* CreateScene(const std::string& _sceneName)
	{
		T* scenePtr = T::create();
		AddScene(_sceneName, scenePtr);

		return scenePtr;
	}

	void AddScene(const std::string& _sceneName, cocos2d::Scene* _scene);
	void RemoveScene(const std::string& _sceneName);
	void SetActiveScene(const std::string& _sceneName);
	cocos2d::Scene* GetActiveScene()
	{
		return m_currentScene;
	}
	bool CheckSceneExist(const std::string& _sceneName);
	void LoadSceneInBackground(const std::string& _sceneName);
	int GetSceneCount()
	{
		return m_sceneCount;
	}
};

NS_MK_END

#endif
