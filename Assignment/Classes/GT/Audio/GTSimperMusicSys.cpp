#include "GTSimperMusicSys.h"
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"
#include "AudioEngine.h"

USING_NS_GT
USING_NS_CC;
using namespace RAPIDJSON_NAMESPACE;
using namespace experimental;

GTSimperMusicSys::GTSimperMusicSys()
{
    // Open the text file from the text file
    FILE *zefp = fopen("Audio/audiodata.json", "r");
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
                    GTSoundData *zeNewData = new GTSoundData();
                    zeNewData->m_fileLocation = it->FindMember("location")->value.GetString();
                    zeNewData->m_Volume = it->FindMember("volume")->value.GetFloat();
                    zeNewData->m_Loop = it->FindMember("loop")->value.GetBool();
                    zeNewData->m_SoundName = it->FindMember("NameID")->value.GetString();
					m_NameSoundMap.insert(std::pair<std::string, GTSoundData*>(zeNewData->m_SoundName, zeNewData));
                    AudioEngine::preload(zeNewData->m_fileLocation);
                }
            }
        }
    }
}


GTSimperMusicSys::~GTSimperMusicSys()
{
    // Then lets save it!
}

int GTSimperMusicSys::playSound(const std::string &_songName)
{
    GTSoundData *zeData = accessSound(_songName);
    if (zeData)
    {
        int zeID = AudioEngine::play2d(zeData->m_fileLocation, zeData->m_Loop, m_MasterVol * zeData->m_Volume);
        zeData->m_AllID.insert(zeID);
        m_AudioIDPlaying.insert(zeID);
        // unfortunately need the lambda function here then it can work!
        AudioEngine::setFinishCallback(zeID,
            [&, zeData, _songName, zeID](int _id, const std::string &_log) {
            m_AudioIDPlaying.erase(zeID);
            zeData->m_AllID.erase(zeID);
            cocos2d::log("Audio finish playing {0}", _log);
        });
        return zeID;
    }
    return SOUND_EFFECT_NOT_FOUND;
}

GTSoundData *GTSimperMusicSys::accessSound(const std::string &_songName)
{
    std::unordered_map<std::string, GTSoundData*>::iterator it = m_NameSoundMap.find(_songName);
    if (it != m_NameSoundMap.end())
    {
        return it->second;
    }
    return nullptr;
}

bool GTSimperMusicSys::setSoundVol(const std::string &_songName, const float &_vol)
{
    GTSoundData *zeSoundData = accessSound(_songName);
    if (zeSoundData)
    {
        zeSoundData->m_Volume = _vol;
        for (std::unordered_set<int>::iterator it = zeSoundData->m_AllID.begin(), end = zeSoundData->m_AllID.end(); it != end; ++it)
        {
            AudioEngine::setVolume(*it, _vol);
        }
        return true;
    }
    return false;
}

bool GTSimperMusicSys::setSoundVol(const int &_songID, const float &_vol)
{
    if (m_AudioIDPlaying.count(_songID))
    {
        AudioEngine::setVolume(_songID, _vol);
    }
    return false;
}

bool GTSimperMusicSys::setSoundVol(GTSoundData* _song, const float &_vol)
{
    _song->m_Volume = _vol;
    if (m_NameSoundMap.count(_song->m_SoundName))
    {
        for (std::unordered_set<int>::iterator it = _song->m_AllID.begin(), end = _song->m_AllID.end(); it != end; ++it)
        {
            AudioEngine::setVolume(*it, _vol);
        }
    }
    return true;
}

bool GTSimperMusicSys::setSoundLoop(const std::string &_songName, const bool &_loop)
{
    GTSoundData *zeSoundData = accessSound(_songName);
    if (zeSoundData && zeSoundData->m_Loop != _loop)
    {
        zeSoundData->m_Loop = _loop;
        for (std::unordered_set<int>::iterator it = zeSoundData->m_AllID.begin(), end = zeSoundData->m_AllID.end(); it != end; ++it)
        {
            AudioEngine::setLoop(*it, _loop);
        }
        return true;
    }
    return false;
}

bool GTSimperMusicSys::setSoundLoop(const int &_songID, const bool &_loop)
{
    if (m_AudioIDPlaying.count(_songID))
    {
        AudioEngine::setLoop(_songID, _loop);
        return true;
    }
    return false;
}

void GTSimperMusicSys::setMasterVol(const float &_vol)
{
    m_MasterVol = _vol;
    for (std::unordered_set<int>::iterator it = m_AudioIDPlaying.begin(), end = m_AudioIDPlaying.end(); it != end; ++it)
    {
        setSoundVol(*it, _vol);
    }
}

float GTSimperMusicSys::getMasterVol()
{
    return m_MasterVol;
}

bool GTSimperMusicSys::stopSound(const std::string &_songName)
{
    GTSoundData *zeSound = accessSound(_songName);
    if (zeSound)
    {
        for (std::unordered_set<int>::iterator listIt = zeSound->m_AllID.begin(), listEnd = zeSound->m_AllID.end(); listIt != listEnd; ++listIt)
        {
            AudioEngine::stop(*listIt);
        }
        return true;
    }
    return false;
}

bool GTSimperMusicSys::stopSound(const int &_songID)
{
    if (m_AudioIDPlaying.count(_songID))
    {
        AudioEngine::stop(_songID);
    }
    return false;
}

bool GTSimperMusicSys::pauseSound(const std::string &_songName)
{
    GTSoundData *zeSound = accessSound(_songName);
    if (zeSound)
    {
        for (std::unordered_set<int>::iterator listIt = zeSound->m_AllID.begin(), listEnd = zeSound->m_AllID.end(); listIt != listEnd; ++listIt)
        {
            AudioEngine::pause(*listIt);
        }
        return true;
    }
    return false;
}

bool GTSimperMusicSys::pauseSound(const int &_songID)
{
    if (m_AudioIDPlaying.count(_songID))
    {
        AudioEngine::pause(_songID);
    }
    return false;
}

bool GTSimperMusicSys::resumeSound(const std::string &_songName)
{
    GTSoundData *zeSound = accessSound(_songName);
    if (zeSound)
    {
        for (std::unordered_set<int>::iterator listIt = zeSound->m_AllID.begin(), listEnd = zeSound->m_AllID.end(); listIt != listEnd; ++listIt)
        {
            AudioEngine::resume(*listIt);
        }
        return true;
    }
    return false;
}

bool GTSimperMusicSys::resumeSound(const int &_songID)
{
    if (m_AudioIDPlaying.count(_songID))
    {
        AudioEngine::resume(_songID);
    }
    return false;
}