#pragma once

#include "cocos2d.h"
#include "../MK/Common/MKMacros.h"
#include "../MK/SceneManagement/MKScene.h"
#include "../MK/Graphics/MKSprite.h"
#include "../GT/Server/GTServerData.h"

USING_NS_CC;
USING_NS_MK

class HighscoreScene : public MKScene
{
    typedef MKScene Super;

protected:
    // Background
    MKSprite * m_Background = nullptr;
    float m_LeftSideToShowPlayer, m_RightSideToShowScore;

    void InitialiseBackground();

    // UI
    void InitialiseUI();

    // Input
    virtual void OnButton(EventCustom * _event) {}
    virtual void OnClick(EventCustom * _event) {}
    virtual void OnAxis(EventCustom * _event) {}

    // wait for highscore
    void GetGlobalHighScore(network::HttpClient* _client, network::HttpResponse* _response);

    void SpawnGlobalHighScoreUI(const std::string &_dataJson);

public:
    HighscoreScene() {}
    virtual ~HighscoreScene();
    CREATE_FUNC(HighscoreScene);

    virtual bool init();
};
