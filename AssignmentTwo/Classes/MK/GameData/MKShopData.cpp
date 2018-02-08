// Include Cocos
#include "cocos2d.h"

// Include MK
#include "MKShopData.h"
#include "../Common/MKJSONHelper.h"

// Include JSON
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"
#include "external/json/document.h"

NS_MK_BEGIN

// Default Save Location
const mkString MKShopData::m_DefaultBackgroundsDataLocation = "GameData/BackgroundsData.json";

// JSON Data Name(s)
const mkString MKShopData::m_BackgroundArrayJSONDataName = "Backgrounds";
const mkString MKShopData::m_BackgroundNameJSONDataName = "Name";
const mkString MKShopData::m_BackgroundCostJSONDataName = "Cost";
const mkString MKShopData::m_BackgroundStaticJSONDataName = "Static";
const mkString MKShopData::m_BackgroundRearJSONDataName = "Rear";
const mkString MKShopData::m_BackgroundMiddleJSONDataName = "Middle";
const mkString MKShopData::m_BackgroundFrontJSONDataName = "Front";

mkBool MKShopData::LoadBackgroundsData(const mkString& _filePath)
{
    RAPIDJSON_NAMESPACE::Document dataDocument;
    MKJSONHelper::LoadFromJSON(dataDocument, _filePath);

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

MKShopItem_Background* MKShopData::GetBackground(const mkString& _backgroundName)
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