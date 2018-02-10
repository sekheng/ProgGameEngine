// Include MK
#include "../MK/SceneManagement/MKSceneManager.h"

#include "../UIClass/UICreator.h"
#include "HighscoreScene.h"
#include "network/HttpClient.h"
#include "external/json/document.h"

using namespace network;
USING_NS_GT
using namespace RAPIDJSON_NAMESPACE;

const static std::string fontName = "fonts/Marker_Felt.ttf";
static bool SCENE_DESTROYED = true;

HighscoreScene::~HighscoreScene()
{
    SCENE_DESTROYED = true;
}

bool HighscoreScene::init()
{
    if (!Super::init())
    {
        return false;
    }
    SCENE_DESTROYED = false;
    InitialiseBackground();
    InitialiseUI();
    // and then send data to server to retrieve highscore!
    GTServerData::GetData(GTServerData::GET10_HIGH_SCORELINK, [&](HttpClient* _client, HttpResponse* _response) {GetGlobalHighScore(_client, _response); });

    return true;
}

void HighscoreScene::InitialiseBackground()
{
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_Background = MKSprite::Create("Textures/Backgrounds/Background_Menu.png", true);
    m_Background->setAnchorPoint(Vec2(0.0f, 0.0f));
    m_Background->setPosition(visibleOrigin.x, visibleOrigin.y);

    // We want the background to fill up the whole screen.
    float backgroundWidth = m_Background->getContentSize().width;
    float backgroundHeight = m_Background->getContentSize().height;
    float backgroundAspectRatio = backgroundWidth / backgroundHeight;

    float desiredWidth = visibleSize.width;
    float desiredHeight = visibleSize.height;

    m_Background->setScale(desiredWidth / backgroundWidth, desiredHeight / backgroundHeight);
    m_Background->SetTextureScale(backgroundWidth / desiredWidth, 1.0f);

    addChild(m_Background);
}

void HighscoreScene::InitialiseUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float halfWidth = visibleSize.width * 0.5f;
    float estimatedHalfPoint = origin.x + halfWidth;
    m_LeftSideToShowPlayer = origin.x + (visibleSize.width * 0.35f);
    m_RightSideToShowScore = origin.x + (visibleSize.width * 0.65f);

    Sprite* backButton = Sprite::create("BackButton.png");

    auto toPrevSceneButton = MKUICreator::GetInstance()->createButton(
        Vec2(backButton->getContentSize().width, visibleSize.height - backButton->getContentSize().height),
        "BackButton.png",
        "BackButton.png",
        "",
        [&](Ref*) -> void
    {
        MKSceneManager::GetInstance()->PopScene();
    },
        (0.1f * visibleSize.height) / backButton->getContentSize().height
        );
    this->addChild(toPrevSceneButton);
    auto label = Label::createWithTTF("Highscore", fontName, 24);
        // position the label on the center of the screen
        label->setPosition(Vec2(estimatedHalfPoint,
            origin.y + visibleSize.height - label->getContentSize().height));
        label->setColor(Color3B::BLACK);
    // add the label as a child to this layer
    this->addChild(label, 1);

    // then display the name and score part!
    float scoreTitleHeight = origin.y + (visibleSize.height * 0.9f);
    auto playerNameTitle = Label::createWithTTF("Name:", fontName, 32);
    // position the label on the center of the screen
    playerNameTitle->setPosition(Vec2(m_LeftSideToShowPlayer, scoreTitleHeight - playerNameTitle->getContentSize().height));
    playerNameTitle->setColor(Color3B::BLACK);
    this->addChild(playerNameTitle, 1);

    auto playerScoreTitle = Label::createWithTTF("Score:", fontName, 32);
    playerScoreTitle->setPosition(Vec2(m_RightSideToShowScore, scoreTitleHeight - playerScoreTitle->getContentSize().height));
    playerScoreTitle->setColor(Color3B::BLACK);
    this->addChild(playerScoreTitle, 1);
}

void HighscoreScene::GetGlobalHighScore(network::HttpClient* _client, network::HttpResponse* _response)
{
    // only when the response succeed!
    if (_response->isSucceed())
    {
        SpawnGlobalHighScoreUI(std::string(_response->getResponseData()->begin(), _response->getResponseData()->end()));
    }
}

void HighscoreScene::SpawnGlobalHighScoreUI(const std::string &_dataJson)
{
    if (SCENE_DESTROYED)
        return;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float highscoreHeight = origin.y + (visibleSize.height * 0.8f);
    // then spawn UI here based on the json data!
    // we need to convert it duh
    Document jsonDoc;
    jsonDoc.Parse(_dataJson.c_str());
    int numberOfHighScores = 0;
    // then iterate through the array!
    for (RAPIDJSON_NAMESPACE::Value::ConstValueIterator it = jsonDoc.Begin(), end = jsonDoc.End(); it != end; ++it)
    {
        // since the class will be like this { "score" : floatnumber, "name" : string }
        float score = it->FindMember("score")->value.GetFloat();
        std::string nameOfPlayer = it->FindMember("name")->value.GetString();
        std::string playerOrderStr = std::to_string(numberOfHighScores + 1) + ". " + nameOfPlayer;
        std::string playerScoreStr = std::to_string((int)score);

        auto playerNameLabel = Label::createWithTTF(playerOrderStr, fontName, 28);
        playerNameLabel->setPosition(Vec2(m_LeftSideToShowPlayer,
            highscoreHeight - (numberOfHighScores * playerNameLabel->getContentSize().height)));
        playerNameLabel->setColor(Color3B::BLACK);
            this->addChild(playerNameLabel, 1);

        auto playerScoreLabel = Label::createWithTTF(playerScoreStr, fontName, 28);
        playerScoreLabel->setPosition(Vec2(m_RightSideToShowScore,
           highscoreHeight - (numberOfHighScores * playerScoreLabel->getContentSize().height)));
        playerScoreLabel->setColor(Color3B::BLACK);
            this->addChild(playerScoreLabel, 1);

        numberOfHighScores++;
    }
}