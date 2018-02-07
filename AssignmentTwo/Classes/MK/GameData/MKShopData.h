#ifndef MK_SHOP_DATA_H
#define MK_SHOP_DATA_H

// Include STL
#include <string>
#include <cmath>
#include <vector>

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKSingletonTemplate.h"
#include "MKShopItem_Background.h"

// Namespaces
using namespace std;

NS_MK_BEGIN

class MKShopData : public MKSingletonTemplate<MKShopData>
{
    friend class MKSingletonTemplate<MKShopData>;

private:
    // Constructor(s) & Destructor
    MKShopData() {}
    virtual ~MKShopData() {}

    // Background Items
    std::vector<MKShopItem_Background> m_Backgrounds;

public:
    // Default Save Location
    static const mkString m_DefaultBackgroundsDataLocation;

    // JSON Data Name(s)
    static const mkString m_BackgroundArrayJSONDataName;
    static const mkString m_BackgroundNameJSONDataName;
    static const mkString m_BackgroundCostJSONDataName;
    static const mkString m_BackgroundStaticJSONDataName;
    static const mkString m_BackgroundRearJSONDataName;
    static const mkString m_BackgroundMiddleJSONDataName;
    static const mkString m_BackgroundFrontJSONDataName;

    mkBool LoadBackgroundsData(const mkString& _filePath = m_DefaultBackgroundsDataLocation);
    MKShopItem_Background* GetBackground(const mkString& _backgroundName);

    inline std::vector<MKShopItem_Background> GetBackgrounds() const { return m_Backgrounds; }
};

NS_MK_END

#endif // MK_SHOP_DATA_H