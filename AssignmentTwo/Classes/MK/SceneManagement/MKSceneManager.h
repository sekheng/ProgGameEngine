#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

//Include Cocos
#include "cocos2d.h"

//Include MKMacros
#include "../../MK/Common/MKAssertions.h"
#include "../../MK/Common/MKMacros.h"
#include "../../MK/Common/MKSingletonTemplate.h"
#include "../../MK/Common/MKPasskey.h"

//Include STL
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <string>

USING_NS_CC;

NS_MK_BEGIN

class MKLoadingScene;
class MKScene;

class MKSceneManager : public MKSingletonTemplate<MKSceneManager>
{
	friend MKSingletonTemplate<MKSceneManager>;

private:
	std::unordered_map<mkString, std::function<MKScene*()> > m_SceneMap;

	// Constructor(s) & Destructor
	MKSceneManager();
	virtual ~MKSceneManager();	

	MKScene* CreateScene(const mkString& _sceneName);

public:
	template <typename T>
	void AddScene(const mkString& _sceneName)
	{
		MK_ASSERTWITHMSG((!CheckSceneExist(_sceneName)), "MKSceneManager::AddScene - There is already a scene with this name!");
		m_SceneMap.insert(std::pair<mkString, std::function<MKScene*()> >(_sceneName, T::create));
	}

	template <typename T>
	void AddSceneWithPhysics(const mkString& _sceneName)
	{
		MK_ASSERTWITHMSG((!CheckSceneExist(_sceneName)), "MKSceneManager::AddScene - There is already a scene with this name!");
		m_SceneMap.insert(std::pair < mkString, std::function<MKScene*()> >(_sceneName, T::createWithPhysics));
	}
	
	void RemoveScene(const mkString& _sceneName)
	{
		std::unordered_map<mkString, std::function<MKScene*()> >::const_iterator iter = m_SceneMap.find(_sceneName);
		MK_ASSERTWITHMSG((iter != m_SceneMap.end()), "MKSceneManager::RemoveScene - Scene not found!");
		m_SceneMap.erase(iter);
	}

	inline unsigned int GetSceneCount() const { return m_SceneMap.size(); }
	inline bool CheckSceneExist(const mkString& _sceneName) const { return m_SceneMap.count(_sceneName) != 0; }

	void ReplaceScene(const mkString& _sceneName);
	void PushScene(const mkString& _sceneName);
	void PopScene();
};

NS_MK_END

#endif
