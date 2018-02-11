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
#include "../MK/Shop/MKShopInterface.h"

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
	void InitialiseShopItemUI();
	void InitialiseShopBackgrounds(MKShopItem_Background* _shopItem);

	void InitPurchaseButtons(MKShopItem_Background* _shopItem);

	//BACKGROUND DISPLAY NODE
	Node* m_ItemDisplayNode = nullptr;

	//LABELS
	Label* m_PlayerCoinsLabel = nullptr;
	Label* m_ShopItemName = nullptr;
	Label* m_ShopItemPrice = nullptr;

	//BUTTONS
	ui::Button* m_buyButton;
	ui::Button* m_equipButton;
	ui::Button* m_equippedButton;

	//DATA
	MKPlayerData* playerData;
	MKGameBackgroundData* shopItem_Background;
	std::vector<MKShopItem_Background> shopItem_list;
    MKSprite* m_Background = nullptr;

	std::vector<ui::Button*> shopItemButtons;

	void UpdateButtonInfo(MKShopItem* _shopItem);

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
