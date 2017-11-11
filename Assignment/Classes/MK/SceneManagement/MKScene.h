#ifndef MK_TESTSCENE_H
#define MK_TESTSCENE_H

// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MK/Common/MKMacros.h"
#include "MK/Input/MKInputManager.h"
#include "MK/Input/MKInput.h"
#include "MK/Common/MKPasskey.h"

USING_NS_CC;

NS_MK_BEGIN

class MKSceneManager;

class MKScene : public cocos2d::Scene
{
	typedef cocos2d::Scene Super;

public:
	MKScene() {}
	virtual ~MKScene() {}

	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(MKScene);	
};

NS_MK_END

#endif