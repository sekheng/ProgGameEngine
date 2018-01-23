#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__

#include "cocos2d.h"

// Include MK
#include "../MK/Common/MKMacros.h"
#include "../MK/Input/MKInputManager.h"
#include "../MK/Input/MKInput.h"
#include "../MK/SceneManagement/MKScene.h"

USING_NS_CC;
USING_NS_MK

class ShopScene : public MKScene
{
	typedef MKScene Super;

public:
	ShopScene() {}
	virtual ~ShopScene() {}

	virtual bool init();

	virtual void OnButton(EventCustom * _event) {}
	virtual void OnClick(EventCustom * _event) {}
	virtual void OnAxis(EventCustom * _event) {}

	// implement the "static create()" method manually
	CREATE_FUNC(ShopScene);
};

#endif
