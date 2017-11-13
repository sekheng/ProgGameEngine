#ifndef MK_LOADINGSCENE_H
#define MK_LOADINGSCENE_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMacros.h"

USING_NS_CC;

NS_MK_BEGIN

class MKLoadingScene : public cocos2d::Scene
{
private:
	EventListenerCustom* m_ProgressListener = nullptr;
	cocos2d::Label* m_ProgressLabel = nullptr;
	const std::string m_SceneToLoad;

	MKLoadingScene(const std::string& _sceneToLoad)
		: m_SceneToLoad(_sceneToLoad) {}
	virtual ~MKLoadingScene() {}

	void problemLoading(const char* _fileName);

	void ProgressUpdateCallback(EventCustom* _event);

public:
	virtual bool init();
	
	// implement the "static create()" method manually
	static MKLoadingScene* create(const std::string& _sceneToLoad)
	{
		MKLoadingScene* pRet = new(std::nothrow) MKLoadingScene(_sceneToLoad);
	    if (pRet && pRet->init())
	    {
	        pRet->autorelease();
	        return pRet;
	    }
	    else
	    {
	        delete pRet;
	        pRet = nullptr;
	        return nullptr;
	    }
	}
};

NS_MK_END

#endif