#ifndef MK_GAME_BACKGROUND_DATA_H
#define MK_GAME_BACKGROUND_DATA_H

// Include STL
#include <string>
#include <cmath>
#include <vector>

// Include MK
#include "../Common/MKMacros.h"
#include "MKGameData.h"
#include "MKShopItem_Background.h"

// Namespaces
using namespace std;

NS_MK_BEGIN

class MKGameBackgroundData : public MKGameData
{
private:
    // Background Items
    std::vector<MKShopItem_Background> m_Backgrounds;

public:
    // Constructor(s) & Destructor
    MKGameBackgroundData() {}
    virtual ~MKGameBackgroundData() {}

    // Default Save Location
    static const mkString m_DefaultDataLocation;

    // JSON Data Name(s)
    static const mkString m_BackgroundArrayJSONDataName;
    static const mkString m_BackgroundNameJSONDataName;
    static const mkString m_BackgroundCostJSONDataName;
    static const mkString m_BackgroundStaticJSONDataName;
    static const mkString m_BackgroundRearJSONDataName;
    static const mkString m_BackgroundMiddleJSONDataName;
    static const mkString m_BackgroundFrontJSONDataName;

    virtual mkBool LoadData(const mkString& _filePath = m_DefaultDataLocation) override;
    virtual mkBool SaveData(const mkString& _filePath = m_DefaultDataLocation) override { return false; }

    MKShopItem_Background* GetBackground(const mkString& _backgroundName);
    inline std::vector<MKShopItem_Background> GetBackgrounds() const { return m_Backgrounds; }
};

NS_MK_END

#endif // MK_GAME_BACKGROUND_DATA_H