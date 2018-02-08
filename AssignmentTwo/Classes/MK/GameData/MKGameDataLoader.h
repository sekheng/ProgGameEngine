#ifndef MK_GAME_DATA_LOADER_H
#define MK_GAME_DATA_LOADER_H

// Include STL
#include <unordered_map>

// Include MK
#include "../Common/MKSingletonTemplate.h"
#include "MKGameData.h"

NS_MK_BEGIN

class MKGameDataLoader : public MKSingletonTemplate<MKGameDataLoader>
{
    friend class MKSingletonTemplate<MKGameDataLoader>;

private:
    // Constructor(s) & Destructor
    MKGameDataLoader() {}
    virtual ~MKGameDataLoader() { DeleteAllGameData(); }

    // Game Data Map
    std::unordered_map<MKGameDataID, MKGameData*> m_GameDataMap;

public:
    void DeleteAllGameData()
    {
        for (std::unordered_map<MKGameDataID, MKGameData*>::iterator i = m_GameDataMap.begin(); i != m_GameDataMap.end(); ++i)
        {
            delete i->second;
        }

        m_GameDataMap.clear();
    }

    template <typename T>
    T* GetGameData()
    {
        MKGameDataID dataId = MKGameData::GetGameDataID<T>();

        // If the data is found, return it.
        std::unordered_map<MKGameDataID, MKGameData*>::iterator mapIter = m_GameDataMap.find(dataId);
        if (mapIter != m_GameDataMap.end())
        {
            return static_cast<T*>(mapIter->second);
        }

        // Otherwise, create a new data and return that instead.
        MKGameData* gameData = new T();
        m_GameDataMap.insert(std::pair<MKGameDataID, MKGameData*>(dataId, gameData));
        return static_cast<T*>(gameData);
    }

    template <typename T>
    void DeleteGameData()
    {
        MKGameDataID dataId = MKGameData::GetGameDataID<T>();

        // If the data is found, delete it.
        std::unordered_map<MKGameDataID, MKGameData*>::iterator mapIter = m_GameDataMap.find(dataId);
        if (mapIter != m_GameDataMap.end())
        {
            delete mapIter->second;
            m_GameDataMap.erase(mapIter);
        }
    }
};

NS_MK_END

#endif // MK_GAME_DATA_LOADER_H