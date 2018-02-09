#ifndef MK_SHOP_INTERFACE_H
#define MK_SHOP_INTERFACE_H

 // Include MK
#include "../Common/MKMacros.h"
#include "../GameData/MKGameDataLoader.h"
#include "../GameData/MKGameBackgroundData.h"
#include "../GameData/MKPlayerData.h"

NS_MK_BEGIN

class MKShopInterface
{
private:
    static MKPlayerData* GetPlayerData()
    {
        return MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
    }

public:
    static bool HasSufficientCoins(const MKShopItem& _item)
    {
        return GetPlayerData()->GetCoins() >= _item.m_Cost;
    }

    // Game Background
    static bool PurchaseGameBackground(const MKShopItem& _item, bool _saveData)
    {
        if (!HasSufficientCoins(_item)) { return false; }

        GetPlayerData()->RemoveCoins(_item.m_Cost);
        GetPlayerData()->AddOwnedBackground(_item.m_Name);
        
        if (_saveData)
        {
            GetPlayerData()->SaveData(GetPlayerData()->GetWritablePath());
        }

        return true;
    }

    static bool EquipGameBackground(const MKShopItem& _item, bool _saveData)
    {
        if (!GetPlayerData()->OwnsBackground(_item.m_Name)) { return false; }

        GetPlayerData()->SetEquippedBackground(_item.m_Name);

        if (_saveData)
        {
            GetPlayerData()->SaveData(GetPlayerData()->GetWritablePath());
        }

        return true;
    }
};

NS_MK_END

#endif // MK_SHOP_INTERFACE_H