// Include Game
#include "StartScreenScene.h"

bool StartScreenScene::init()
{
    if (!Super::init()) { return false; }

    InitialiseSkyBackground();
    InitialiseLogo();
    InitialiseStartLabel();
    InitialiseInput();

    return true;
}

void StartScreenScene::onEnter()
{
    Super::onEnter();
    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_STARTSCENE);
}

void StartScreenScene::onExit()
{
    Super::onExit();
    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_DEFAULT);
}

void StartScreenScene::InitialiseStartLabel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();

    float fontSize = visibleSize.height * 0.05f;
    m_StartLabel = cocos2d::Label::createWithTTF("Touch screen to start.", "Fonts/Marker_Felt.ttf", fontSize);
    m_StartLabel->setPosition(Vec2(visibleOrigin.x + visibleSize.width * 0.5f,
        visibleOrigin.y + visibleSize.height * 0.25f));

    m_StartLabel->setTextColor(Color4B::BLACK);

    // add the label as a child to this layer
    this->addChild(m_StartLabel);
}

void StartScreenScene::InitialiseLogo()
{
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Size desiredSize(visibleSize.height, visibleSize.height * 0.5f);
    m_Logo = MKSprite::CreateWithSize("Textures/UI/Logo.png", desiredSize, false);
    m_Logo->setAnchorPoint(Vec2(0.5f, 1.0f));
    m_Logo->setPosition(Vec2(visibleOrigin.x + visibleSize.width * 0.5f,
        visibleOrigin.y + visibleSize.height * 0.9f));

    addChild(m_Logo);
}

void StartScreenScene::InitialiseSkyBackground()
{
    Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();

    m_SkyBackground = MKSprite::Create("Textures/Backgrounds/Background_Sky.png", true);
    m_SkyBackground->setAnchorPoint(Vec2(0.0f, 0.0f));
    m_SkyBackground->setPosition(visibleOrigin.x, visibleOrigin.y);

    // We want the background to fill up the whole screen.
    float backgroundWidth = m_SkyBackground->getContentSize().width;
    float backgroundHeight = m_SkyBackground->getContentSize().height;
    float backgroundAspectRatio = backgroundWidth / backgroundHeight;

    float desiredWidth = visibleSize.width;
    float desiredHeight = visibleSize.height;

    m_SkyBackground->setScale(desiredWidth / backgroundWidth, desiredHeight / backgroundHeight);
    m_SkyBackground->SetTextureScale(backgroundWidth / desiredWidth, 1.0f);

    addChild(m_SkyBackground);
}

void StartScreenScene::OnClick(EventCustom * _event)
{
    MKInputClick* input = static_cast<MKInputClick*>(_event->getUserData());

    if (input->m_InputName == START_GAME)
    {
        if (input->m_ButtonState == MKInputClick::ButtonState::RELEASE)
        {
            Deinitialise();
            MKSceneManager::GetInstance()->ReplaceScene("MainMenuScene");
        }
    }
}