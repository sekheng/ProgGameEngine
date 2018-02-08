#ifndef MK_PLAYER_DATA_H
#define MK_PLAYER_DATA_H

// Include STL
#include <string>
#include <cmath>
#include <vector>

// Include MK
#include "../Common/MKMacros.h"
#include "MKGameData.h"

// Namespaces
using namespace std;

NS_MK_BEGIN

class MKPlayerData : public MKGameData
{
private:
    // Highscore and Coins
    mkU64 m_Highscore = 0;
    mkU64 m_Coins = 0;
    mkString m_EquippedBackground;
    vector<mkString> m_OwnedBackgrounds;

public:
    // Contructor(s) & Destructor
    MKPlayerData() {}
    virtual ~MKPlayerData() {}

    // JSON Data Name(s)
    static const mkString m_HighscoreJSONDataName;
    static const mkString m_CoinJSONDataName;
    static const mkString m_EquippedBackgroundJSONDataName;
    static const mkString m_OwnedBackgroundsJSONDataName;

    virtual mkBool LoadData(const mkString& _filePath) override;
    virtual mkBool SaveData(const mkString& _filePath) override;

    virtual mkString GetWritablePath() const override;
    virtual mkString GetCachedPath() const override;

    inline mkU64 GetHighscore() const { return m_Highscore; }
    inline void SetHighscore(mkU64 _highscore) { m_Highscore = _highscore; }

    inline mkU64 GetCoins() const { return m_Coins; }
    inline void SetCoins(mkU64 _coins) { m_Coins = _coins; }
    inline void AddCoins(mkU64 _coins) { m_Coins += _coins; }
    inline void RemoveCoins(mkU64 _coins) { m_Coins -= _coins; }
    
    inline const mkString& GetEquippedBackground() const { return m_EquippedBackground; }
    inline void SetEquippedBackground(const mkString& _backgroundName) { m_EquippedBackground = _backgroundName; }

    inline vector<mkString> GetOwnedBackgrounds() const { return m_OwnedBackgrounds; }
    inline void SetOwnedBackgrounds(vector<mkString> _backgroundNames) { m_OwnedBackgrounds = _backgroundNames; }
    inline void AddOwnedBackground(const mkString& _backgroundName) { m_OwnedBackgrounds.push_back(_backgroundName); }
    bool OwnsBackground(const mkString& _backgroundName);
};

NS_MK_END

#endif // MK_PLAYER_DATA_H