#pragma once

#include "cocos2d.h"
#include "MK/Common/MKSingletonTemplate.h"
#include "external/json/document.h"

namespace GinTama {
    struct SoundData
    {
        float m_Volume;
        int m_AudioID = -1;
        std::string m_fileLocation;
        bool m_Loop;
    };

    class SimperMusicSys : public MinamiKotori::MKSingletonTemplate<SimperMusicSys>
    {
    public:
        bool playSound(const std::string &_songName);

        SoundData* accessSound(const std::string &_songName);

        bool setSoundVol(const std::string &_songName, const float &_vol);
        bool setSoundLoop(const std::string &_songName, const bool &_loop);

        void setMasterVol(const float &_vol);
        float getMasterVol();

        SimperMusicSys();
    protected:
        virtual ~SimperMusicSys();

        void resetSoundID(int _id, const std::string &_log);

        float m_MasterVol;
        RAPIDJSON_NAMESPACE::Document m_MusicDocs;
        std::unordered_map<std::string, SoundData*> m_NameSoundMap;
    };
};
