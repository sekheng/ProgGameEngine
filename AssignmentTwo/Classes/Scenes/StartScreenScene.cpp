// Include Game
#include "StartScreenScene.h"

bool StartScreenScene::init()
{
    if (!Super::init()) { return false; }

    InitialiseSkyBackground();
    InitialiseLogo();
    InitialiseStartLabel();
    InitialiseInput();
#ifdef SDKBOX_ENABLED
    sdkbox::PluginFacebook::setListener(this);
    if (!sdkbox::PluginFacebook::isLoggedIn())
    {
        sdkbox::PluginFacebook::login();
    }
#endif
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

    float fontSize = visibleSize.height * 0.05f;
    m_StartLabel = cocos2d::Label::createWithTTF("Touch screen to start.", "fonts/Marker_Felt.ttf", fontSize);
    m_StartLabel->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.25f));
    m_StartLabel->setTextColor(Color4B::BLACK);

    // add the label as a child to this layer
    this->addChild(m_StartLabel);
}

void StartScreenScene::InitialiseLogo()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();

    Size desiredSize(visibleSize.height * 0.75f, visibleSize.height * 0.375f);
    m_Logo = MKSprite::Create("Textures/UI/Logo.png", false);
    m_Logo->setScaleX(desiredSize.width / m_Logo->getContentSize().width);
    m_Logo->setScaleY(desiredSize.height / m_Logo->getContentSize().height);
    m_Logo->setAnchorPoint(Vec2(0.5f, 1.0f));
    m_Logo->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height * 0.8f));

    this->addChild(m_Logo);
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

void StartScreenScene::onLogin(bool isLogin, const std::string& msg)
{
    CCLOG("FB login: %u", isLogin);
    //sdkbox::PluginFacebook::requestPublishPermissions({sdkbox::FB_PERM_PUBLISH_POST});
}
void StartScreenScene::onSharedSuccess(const std::string& message)
{
    CCLOG("Sharing is successful according to %s", message.c_str());
}
void StartScreenScene::onSharedFailed(const std::string& message)
{
    CCLOG("Sharing is failed. plz fix this bug!");
}
void StartScreenScene::onSharedCancel()
{
    CCLOG("sharing is cancel");
}
void StartScreenScene::onAPI(const std::string& key, const std::string& jsonData)
{
    
}
void StartScreenScene::onPermission(bool isLogin, const std::string& msg)
{
    // TODO, remove this sharing post example
    if (isLogin)
    {
        sdkbox::FBShareInfo info;
        info.type  = sdkbox::FB_LINK;
        info.link  = "http://www.cocos2d-x.org";
        info.title = "cocos2d-x";
        info.text  = "Sek Heng here trying out sharing for COCOS2DX!!!!!!";
        info.image = "http://cocos2d-x.org/images/logo.png";
        sdkbox::PluginFacebook::dialog(info);
    }
}
void StartScreenScene::onFetchFriends(bool ok, const std::string& msg)
{
    
}
void StartScreenScene::onRequestInvitableFriends( const sdkbox::FBInvitableFriendsInfo& friends )
{
    
}
void StartScreenScene::onInviteFriendsWithInviteIdsResult( bool result, const std::string& msg )
{
    
}
void StartScreenScene::onInviteFriendsResult( bool result, const std::string& msg )
{
    
}

void StartScreenScene::onGetUserInfo( const sdkbox::FBGraphUser& userInfo )
{
    
}

