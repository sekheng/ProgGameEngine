#ifndef MK_GAME_DATA_H
#define MK_GAME_DATA_H

// Include STL
#include <type_traits>

// Include MK
#include "../Common/MKMacros.h"
#include "../Common/MKAssertions.h"

NS_MK_BEGIN

typedef mkU32 MKGameDataID;

class MKGameData
{
private:
    // Static Helper Function(s)
    static MKGameDataID GenerateGameDataID()
    {
        static MKGameDataID currentID = 0;
        return currentID++;
    }

public:
    // Constructor(s) & Destructor
    MKGameData() {}
    virtual ~MKGameData() {}

    // Virtual Function(s)
    virtual mkBool LoadData(const mkString& _filePath) = 0;
    virtual mkBool SaveData(const mkString& _filePath) = 0;

    virtual mkString GetWritablePath() const = 0;
    virtual mkString GetCachedPath() const = 0;

    // Static Interface Function(s)
    template <typename T>
    static MKGameDataID GetGameDataID()
    {
        MK_ASSERT((std::is_base_of<MKGameData, T>::value));

        static MKGameDataID m_dataID = MKGameData::GenerateGameDataID();
        return m_dataID;
    }
};

NS_MK_END

#endif // MK_GAME_DATA_H