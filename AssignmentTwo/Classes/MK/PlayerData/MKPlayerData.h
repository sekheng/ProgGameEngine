#ifndef MK_PLAYER_DATA_H
#define MK_PLAYER_DATA_H

// Includes Cocos
#include "cocos2d.h"

// Include STL
// Include STL
#include <string>
#include <cmath>

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKSingletonTemplate.h"

USING_NS_CC;
using namespace std;

NS_MK_BEGIN

class MKPlayerData : public MKSingletonTemplate<MKPlayerData>
{
    friend class MKSingletonTemplate<MKPlayerData>;

private:
    // Contructor(s) & Destructor
    MKPlayerData() {}
    virtual ~MKPlayerData() {}

protected:
    mkU64 m_Highscore = 0;
    mkU64 m_Coins = 0;

public:
    // Default Save Location
    static const mkString m_DefaultSaveLocation;

    // JSON Data Name(s)
    static const mkString m_HighscoreJSONDataName;
    static const mkString m_CoinJSONDataName;

    static const mkU16 m_MaxFileCharacters = 65535;

    mkBool LoadPlayerData(const mkString& _filePath);
    mkBool SavePlayerData(const mkString& _filePath);

    inline mkU64 GetHighscore() const { return m_Highscore; }
    inline void SetHighscore(mkU64 _highscore) { m_Highscore = _highscore; }
    inline mkU64 GetCoins() const { return m_Coins; }
    inline void SetCoins(mkU64 _coins) { m_Coins = _coins; }
};

NS_MK_END

#endif // MK_PLAYER_DATA_H