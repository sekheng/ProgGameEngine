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

// JSON Data Name(s)
const mkString MKPlayerData::m_HighscoreJSONDataName = "Highscore";
const mkString MKPlayerData::m_CoinJSONDataName = "Coin";
const mkString MKPlayerData::m_EquippedBackgroundJSONDataName = "Equipped_Background";
const mkString MKPlayerData::m_OwnedBackgroundsJSONDataName = "Owned_Backgrounds";

mkBool MKPlayerData::LoadData(const mkString& _filePath)
{
    RAPIDJSON_NAMESPACE::Document dataDocument;
    if (!MKJSONHelper::LoadFromJSON(dataDocument, _filePath))
    {
        return false;
    }

    // Load Highscore
    auto highscore = dataDocument.FindMember(m_HighscoreJSONDataName.c_str());
    if (!highscore->value.IsUint64()) { return false; }
    SetHighscore(highscore->value.GetUint64());

    // Load Coins
    auto coins = dataDocument.FindMember(m_CoinJSONDataName.c_str());
    if (!coins->value.IsUint64()) { return false; }
    SetCoins(coins->value.GetUint64());

    // Load Equipped Background
    auto equippedBackground = dataDocument.FindMember(m_EquippedBackgroundJSONDataName.c_str());
    if (!equippedBackground->value.IsString()) { return false; }
    SetEquippedBackground(equippedBackground->value.GetString());

    // Load Owned Backgrounds
    auto ownedBackgrounds = dataDocument.FindMember(m_OwnedBackgroundsJSONDataName.c_str());
    if (!ownedBackgrounds->value.IsArray()) { return false; }
    m_OwnedBackgrounds.clear();
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator i = ownedBackgrounds->value.Begin(); i != ownedBackgrounds->value.End(); ++i)
    {
        if (!i->IsString()) { return false; }
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
    mkBool result = MKJSONHelper::JsonToFile(dataDocument, _filePath);

    return result;
}

mkString MKPlayerData::GetWritablePath() const
{
    return cocos2d::FileUtils::getInstance()->getWritablePath() + "PlayerData.json";
}

mkString MKPlayerData::GetCachedPath() const
{
    return cocos2d::FileUtils::getInstance()->fullPathForFilename("GameData/PlayerData.json");
}

bool MKPlayerData::OwnsBackground(const mkString& _backgroundName)
{
    for (mkU32 i = 0; i < m_OwnedBackgrounds.size(); ++i)
    {
        if (m_OwnedBackgrounds[i] == _backgroundName)
        {
            return true;
        }
    }

    return false;
}

NS_MK_END