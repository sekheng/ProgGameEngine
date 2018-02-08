// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MKPlayerData.h"
#include "../Common/MKJSONHelper.h"

// Include JSON
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"
#include "external/json/document.h"

NS_MK_BEGIN

// Default Save Location
const mkString MKPlayerData::m_DefaultDataLocation = "GameData/PlayerData.json";

// JSON Data Name(s)
const mkString MKPlayerData::m_HighscoreJSONDataName = "Highscore";
const mkString MKPlayerData::m_CoinJSONDataName = "Coin";
const mkString MKPlayerData::m_EquippedBackgroundJSONDataName = "Equipped_Background";
const mkString MKPlayerData::m_OwnedBackgroundsJSONDataName = "Owned_Backgrounds";

mkBool MKPlayerData::LoadData(const mkString& _filePath)
{
    RAPIDJSON_NAMESPACE::Document dataDocument;
    MKJSONHelper::LoadFromJSON(dataDocument, _filePath);

    // Load Highscore
    auto highscore = dataDocument.FindMember(m_HighscoreJSONDataName.c_str());
    MK_ASSERT(highscore->value.IsUint64());
    SetHighscore(highscore->value.GetUint64());

    // Load Coins
    auto coins = dataDocument.FindMember(m_CoinJSONDataName.c_str());
    MK_ASSERT(coins->value.IsUint64());
    SetCoins(coins->value.GetUint64());

    // Load Equipped Background
    auto equippedBackground = dataDocument.FindMember(m_EquippedBackgroundJSONDataName.c_str());
    MK_ASSERT(equippedBackground->value.IsString());
    SetEquippedBackground(equippedBackground->value.GetString());

    // Load Owned Backgrounds
    auto ownedBackgrounds = dataDocument.FindMember(m_OwnedBackgroundsJSONDataName.c_str());
    MK_ASSERT(ownedBackgrounds->value.IsArray());
    m_OwnedBackgrounds.clear();
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator i = ownedBackgrounds->value.Begin(); i != ownedBackgrounds->value.End(); ++i)
    {
        MK_ASSERT(i->IsString());
        AddOwnedBackground(i->GetString());
    }

    return true;
}

mkBool MKPlayerData::SaveData(const mkString& _filePath)
{
    RAPIDJSON_NAMESPACE::Document dataDocument;
    dataDocument.SetObject();

    // Save Highscore
    RAPIDJSON_NAMESPACE::Value highscoreName;
    highscoreName.SetString(m_HighscoreJSONDataName.c_str(), m_HighscoreJSONDataName.length());
    RAPIDJSON_NAMESPACE::Value highscore;
    highscore.SetUint64(m_Highscore);
    dataDocument.AddMember(highscoreName, highscore, dataDocument.GetAllocator());

    // Save Coins
    RAPIDJSON_NAMESPACE::Value coinsName;
    coinsName.SetString(m_CoinJSONDataName.c_str(), m_CoinJSONDataName.length());
    RAPIDJSON_NAMESPACE::Value coins;
    coins.SetUint64(m_Coins);
    dataDocument.AddMember(coinsName, coins, dataDocument.GetAllocator());

    // Save Equipped Background
    RAPIDJSON_NAMESPACE::Value equippedBackgroundName;
    equippedBackgroundName.SetString(m_EquippedBackgroundJSONDataName.c_str(), m_EquippedBackgroundJSONDataName.length());
    RAPIDJSON_NAMESPACE::Value equippedBackground;
    equippedBackground.SetString(m_EquippedBackground.c_str(), m_EquippedBackground.length());
    dataDocument.AddMember(equippedBackgroundName, equippedBackground, dataDocument.GetAllocator());

    // Save Owned Backgrounds
    RAPIDJSON_NAMESPACE::Value ownedBackgroundsName;
    ownedBackgroundsName.SetString(m_OwnedBackgroundsJSONDataName.c_str(), m_OwnedBackgroundsJSONDataName.length());
    RAPIDJSON_NAMESPACE::Value ownedBackgrounds;
    ownedBackgrounds.SetArray();
    for (std::vector<mkString>::iterator i = m_OwnedBackgrounds.begin(); i != m_OwnedBackgrounds.end(); ++i)
    {
        RAPIDJSON_NAMESPACE::Value background;
        background.SetString(i->c_str(), i->length());
        ownedBackgrounds.PushBack(background, dataDocument.GetAllocator());
    }
    dataDocument.AddMember(ownedBackgroundsName, ownedBackgrounds, dataDocument.GetAllocator());

    // Open the file and write to it.
    return MKJSONHelper::JsonToFile(dataDocument, _filePath);
}

NS_MK_END