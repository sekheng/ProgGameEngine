#pragma once

#include "cocos2d.h"
#include "MK/Common/MKSingletonTemplate.h"
#include "external/json/document.h"
#include <unordered_set>

namespace GinTama {
    const static int SOUND_EFFECT_NOT_FOUND = -1;

    struct GTSoundData
    {
        float m_Volume;
        std::unordered_set<int> m_AllID;
        std::string m_fileLocation;
        std::string m_SoundName;
        bool m_Loop;
    };

    class GTSimperMusicSys : public MinamiKotori::MKSingletonTemplate<GTSimperMusicSys>
    {
    public:
        int playSound(const std::string &_songName);

        GTSoundData* accessSound(const std::string &_songName);

        bool stopSound(const std::string &_songName);
        bool stopSound(const int &_songID);

        bool pauseSound(const std::string &_songName);
        bool pauseSound(const int &_songID);

        bool resumeSound(const std::string &_songName);
        bool resumeSound(const int &_songID);

        bool setSoundVol(const std::string &_songName, const float &_vol);
        bool setSoundVol(const int &_songID, const float &_vol);
        bool setSoundVol(GTSoundData* _song, const float &_vol);

        bool setSoundLoop(const std::string &_songName, const bool &_loop);
        bool setSoundLoop(const int &_songID, const bool &_loop);

        void setMasterVol(const float &_vol);
        float getMasterVol();

        GTSimperMusicSys();
    protected:
        virtual ~GTSimperMusicSys();

        float m_MasterVol;
        RAPIDJSON_NAMESPACE::Document m_MusicDocs;
        std::unordered_map<std::string, GTSoundData*> m_NameSoundMap;
        //std::map<std::string, std::list<GTSoundData*> > m_NamePlayingSound;
        std::unordered_set<int> m_AudioIDPlaying;
    };
};
