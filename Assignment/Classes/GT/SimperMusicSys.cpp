#include "SimperMusicSys.h"
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"
#include "AudioEngine.h"

using namespace SakataGintoki;
using namespace RAPIDJSON_NAMESPACE;
using namespace cocos2d;
using namespace experimental;

SimperMusicSys::SimperMusicSys()
{
    // Open the text file from the text file
    FILE *zefp = fopen("Audio/audiodata.txt", "r");
    // this means there can only be an array of 65536 characters!
    char zeBuffer[65536];
    FileReadStream zeIS(zefp, zeBuffer, sizeof(zeBuffer));
    m_MusicDocs.ParseStream(zeIS);
    fclose(zefp);
    if (m_MusicDocs.HasMember("master_vol"))
    {
        m_MasterVol = m_MusicDocs["master_vol"].GetFloat();
    }
    if (m_MusicDocs.HasMember("audiodata_arr"))
    {
        const RAPIDJSON_NAMESPACE::Value &zeAudioData = m_MusicDocs["audiodata_arr"];
        if (zeAudioData.IsArray())
        {
            for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator it = zeAudioData.Begin(), end = zeAudioData.End(); it != end; ++it)
            {
                if (it->IsObject())
                {
                    // This will have to look hardcoded unfortunately
                    SoundData *zeNewData = new SoundData();
                    zeNewData->m_fileLocation = it->FindMember("location")->value.GetString();
                    zeNewData->m_Volume = it->FindMember("volume")->value.GetFloat();
                    zeNewData->m_Loop = it->FindMember("loop")->value.GetBool();
                }
            }
        }
    }
}


SimperMusicSys::~SimperMusicSys()
{
    // Then lets save it!
}

bool SimperMusicSys::playSound(const std::string &_songName)
{
    SoundData *zeData = accessSound(_songName);
    if (zeData)
    {
        zeData->m_AudioID = AudioEngine::play2d(zeData->m_fileLocation, zeData->m_Loop, m_MasterVol * zeData->m_Volume);
        // unfortunately need the lambda function here then it can work!
        AudioEngine::setFinishCallback(zeData->m_AudioID, 
            [zeData](int _id, const std::string &_log) {
            zeData->m_AudioID = -1;
            cocos2d::log("Audio finish playing {0}", _log);
        });
        return true;
    }
    return false;
}

SoundData *SimperMusicSys::accessSound(const std::string &_songName)
{
    std::unordered_map<std::string, SoundData*>::iterator it = m_NameSoundMap.find(_songName);
    if (it != m_NameSoundMap.end())
    {
        return it->second;
    }
    return nullptr;
}

bool SimperMusicSys::setSoundVol(const std::string &_songName, const float &_vol)
{
    SoundData *zeSoundData = accessSound(_songName);
    if (zeSoundData)
    {
        zeSoundData->m_Volume = _vol;
        if (zeSoundData->m_AudioID >= 0)
        {
            AudioEngine::setVolume(zeSoundData->m_AudioID, _vol);
        }
        return true;
    }
    return false;
}

bool SimperMusicSys::setSoundLoop(const std::string &_songName, const bool &_loop)
{
    SoundData *zeSoundData = accessSound(_songName);
    if (zeSoundData && zeSoundData->m_Loop != _loop)
    {
        zeSoundData->m_Loop = _loop;

        return true;
    }
    return false;
}

void SimperMusicSys::setMasterVol(const float &_vol)
{
    m_MasterVol = _vol;
}

float SimperMusicSys::getMasterVol()
{
    return m_MasterVol;
}