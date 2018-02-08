#ifndef MK_PLAYER_DATA_H
#define MK_PLAYER_DATA_H

// Include STL
#include <string>
#include <cmath>
#include <vector>

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKSingletonTemplate.h"

// Namespaces
using namespace std;

NS_MK_BEGIN

class MKPlayerData : public MKSingletonTemplate<MKPlayerData>
{
    friend class MKSingletonTemplate<MKPlayerData>;

private:
    // Contructor(s) & Destructor
    MKPlayerData() {}
    virtual ~MKPlayerData() {}

    // Highscore and Coins
    mkU64 m_Highscore = 0;
    mkU64 m_Coins = 0;
    mkString m_EquippedBackground;
    vector<mkString> m_OwnedBackgrounds;

public:
    // Default Save Location
    static const mkString m_DefaultDataLocation;

    // JSON Data Name(s)
    static const mkString m_HighscoreJSONDataName;
    static const mkString m_CoinJSONDataName;
    static const mkString m_EquippedBackgroundJSONDataName;
    static const mkString m_OwnedBackgroundsJSONDataName;

    mkBool LoadData(const mkString& _filePath = m_DefaultDataLocation);
    mkBool SaveData(const mkString& _filePath = m_DefaultDataLocation);

    inline mkU64 GetHighscore() const { return m_Highscore; }
    inline void SetHighscore(mkU64 _highscore) { m_Highscore = _highscore; }

    inline mkU64 GetCoins() const { return m_Coins; }
    inline void SetCoins(mkU64 _coins) { m_Coins = _coins; }
    inline void AddCoins(mkU64 _coins) { m_Coins += _coins; }
    inline void RemoveCoins(mkU64 _coins) { m_Coins -= _coins; }
    
    inline const mkString& GetEquippedBackground() const { return m_EquippedBackground; }
    inline void SetEquippedBackground(const mkString& _equippedBackground) { m_EquippedBackground = _equippedBackground; }
    
    inline vector<mkString> GetOwnedBackgrounds() const { return m_OwnedBackgrounds; }
    inline void SetOwnedBackgrounds(vector<mkString> _ownedBackgrounds) { m_OwnedBackgrounds = _ownedBackgrounds; }
    inline void AddOwnedBackground(mkString _ownedBackground) { m_OwnedBackgrounds.push_back(_ownedBackground); }
};

NS_MK_END

#endif // MK_PLAYER_DATA_H