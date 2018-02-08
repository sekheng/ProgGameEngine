// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MKGameBackgroundData.h"
#include "../Common/MKJSONHelper.h"

// Include JSON
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"
#include "external/json/document.h"

NS_MK_BEGIN

// JSON Data Name(s)
const mkString MKGameBackgroundData::m_BackgroundArrayJSONDataName = "Backgrounds";
const mkString MKGameBackgroundData::m_BackgroundNameJSONDataName = "Name";
const mkString MKGameBackgroundData::m_BackgroundCostJSONDataName = "Cost";
const mkString MKGameBackgroundData::m_BackgroundStaticJSONDataName = "Static";
const mkString MKGameBackgroundData::m_BackgroundRearJSONDataName = "Rear";
const mkString MKGameBackgroundData::m_BackgroundMiddleJSONDataName = "Middle";
const mkString MKGameBackgroundData::m_BackgroundFrontJSONDataName = "Front";

mkBool MKGameBackgroundData::LoadData(const mkString& _filePath)
{
    RAPIDJSON_NAMESPACE::Document dataDocument;
    if (!MKJSONHelper::LoadFromJSON(dataDocument, _filePath))
    {
        return false;
    }

    // Load Backgrounds
    m_Backgrounds.clear();
    auto backgroundArray = dataDocument.FindMember(m_BackgroundArrayJSONDataName.c_str());
    MK_ASSERT(backgroundArray->value.IsArray());
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator i = backgroundArray->value.Begin(), end = backgroundArray->value.End(); i != end; ++i)
    {
        auto itemName = i->FindMember(m_BackgroundNameJSONDataName.c_str());
        MK_ASSERT(itemName->value.IsString());

        auto itemCost = i->FindMember(m_BackgroundCostJSONDataName.c_str());
        MK_ASSERT(itemCost->value.IsUint());

        auto staticBackgroundFile = i->FindMember(m_BackgroundStaticJSONDataName.c_str());
        MK_ASSERT(staticBackgroundFile->value.IsString());

        auto rearBackgroundFile = i->FindMember(m_BackgroundRearJSONDataName.c_str());
        MK_ASSERT(rearBackgroundFile->value.IsString());

        auto middleBackgroundFile = i->FindMember(m_BackgroundMiddleJSONDataName.c_str());
        MK_ASSERT(middleBackgroundFile->value.IsString());

        auto frontBackgroundFile = i->FindMember(m_BackgroundFrontJSONDataName.c_str());
        MK_ASSERT(frontBackgroundFile->value.IsString());

        MKShopItem_Background shopItem(itemName->value.GetString(), itemCost->value.GetUint());
        shopItem.SetBackgroundFile(MKShopItem_Background::BackgroundLayers::STATIC, staticBackgroundFile->value.GetString());
        shopItem.SetBackgroundFile(MKShopItem_Background::BackgroundLayers::REAR, rearBackgroundFile->value.GetString());
        shopItem.SetBackgroundFile(MKShopItem_Background::BackgroundLayers::MIDDLE, middleBackgroundFile->value.GetString());
        shopItem.SetBackgroundFile(MKShopItem_Background::BackgroundLayers::FRONT, frontBackgroundFile->value.GetString());

        m_Backgrounds.push_back(shopItem);
    }

    return true;
}

mkString MKGameBackgroundData::GetWritablePath() const
{
    return "";
}

mkString MKGameBackgroundData::GetCachedPath() const
{
    return cocos2d::FileUtils::getInstance()->fullPathForFilename("GameData/BackgroundsData.json");
}

MKShopItem_Background* MKGameBackgroundData::GetBackground(const mkString& _backgroundName)
{
    for (std::vector<MKShopItem_Background>::iterator i = m_Backgrounds.begin(); i != m_Backgrounds.end(); ++i)
    {
        if (i->m_Name == _backgroundName)
        {
            return &(*i);
        }
    }

    return nullptr;
}

NS_MK_END