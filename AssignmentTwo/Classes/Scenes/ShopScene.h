#ifndef __SHOP_SCENE_H__
#define __SHOP_SCENE_H__

#include "cocos2d.h"

// Include MK
#include "../MK/Common/MKMacros.h"
#include "../MK/Input/MKInputManager.h"
#include "../MK/Input/MKInput.h"
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/GameData/MKGameBackgroundData.h"
#include "../MK/GameData/MKGameDataLoader.h"
#include "../MK/GameData//MKPlayerData.h"

#include "../UIClass/UICreator.h"

USING_NS_CC;
USING_NS_MK

class ShopScene : public MKScene
{
	typedef MKScene Super;

protected:
    // Background
    void InitialiseBackground();
	void InitialiseUI();
	void InitialisePlayerCoinUI();

	MKPlayerData* playerData;
	Label* m_PlayerCoinsLabel = nullptr;
	//MKGameBackgroundData* shopItem_Background;
    MKSprite* m_Background = nullptr;

	std::vector<ui::Button*> shopItemButtons;

    // Input
    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event) {}
    virtual void OnAxis(EventCustom * _event) {}

public:
    // Constructor(s) & Destructor
	ShopScene() {}
	virtual ~ShopScene() {}
    CREATE_FUNC(ShopScene);

	virtual bool init();
	virtual void update(float _deltaTime) override;
};

#endif
