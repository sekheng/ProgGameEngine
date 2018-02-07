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
const mkString MKPlayerData::m_DefaultSaveLocation = "PlayerData/PlayerData.json";

// JSON Data Name(s)
const mkString MKPlayerData::m_HighscoreJSONDataName = "Highscore";
const mkString MKPlayerData::m_CoinJSONDataName = "Coin";

mkBool MKPlayerData::LoadPlayerData(const mkString& _filePath)
{
    FILE* inputFile = fopen(FileUtils::getInstance()->fullPathForFilename(_filePath).c_str(), "r");
    if (inputFile == nullptr)
    {
        CCLOG("MKPlayerData::LoadPlayerData - Unable to open file!");
        return false;
    }
    // The number of characters in the file must not exceed MAX_FILE_CHARACTERS.
    char buffer[m_MaxFileCharacters];
    RAPIDJSON_NAMESPACE::FileReadStream fileReadStream(inputFile, buffer, sizeof(buffer));
    RAPIDJSON_NAMESPACE::Document dataDocument;
    dataDocument.ParseStream(fileReadStream);
    fclose(inputFile);

    // Load Highscore
    auto highscore = dataDocument.FindMember(m_HighscoreJSONDataName.c_str());
    SetHighscore(highscore->value.GetUint64());

    // Load Coins
    auto coins = dataDocument.FindMember(m_CoinJSONDataName.c_str());
    SetCoins(coins->value.GetUint64());

    return true;
}

mkBool MKPlayerData::SavePlayerData(const mkString& _filePath)
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

    // Open the file and write to it.
    return MKJSONHelper::JsonToFile(dataDocument, _filePath);
}

NS_MK_END