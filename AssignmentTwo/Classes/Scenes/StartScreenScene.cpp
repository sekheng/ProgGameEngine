// Include Game
#include "StartScreenScene.h"

// Include MK
#include "../MK/GameData/MKGameDataLoader.h"
#include "../MK/GameData/MKPlayerData.h"
#include "../MK/GameData/MKGameBackgroundData.h"
#include "../GT/Facebook/GTFacebookHelper.h"

USING_NS_GT

bool StartScreenScene::init()
{
    if (!Super::init()) { return false; }

    // Do this in Start Screen as it only needs to be loaded once.
    InitialiseData();
    InitialiseBackground();
    InitialiseLogo();
    InitialiseStartLabel();
#ifndef WIN32
#ifdef SDKBOX_ENABLED
    sdkbox::PluginFacebook::setListener(this);
    if (!sdkbox::PluginFacebook::isLoggedIn())
    {
        sdkbox::PluginFacebook::login();
    }
#endif
#endif

    return true;
}

void StartScreenScene::onEnter()
{
    Super::onEnter();
    InitialiseInput();
    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_STARTSCENE);
}

void StartScreenScene::onExit()
{
    Super::onExit();
    DeinitialiseInput();
    MKInputManager::GetInstance()->SetCurrentContext(MK_INPUT_CONTEXT_DEFAULT);
}

void StartScreenScene::InitialiseData()
{
    // Load Game Backgrounds
    {
        MKGameBackgroundData* gameBackgroundData = MKGameDataLoader::GetInstance()->GetGameData<MKGameBackgroundData>();
        gameBackgroundData->LoadData(gameBackgroundData->GetCachedPath());
    }

    // Load Player
    {
        // Check if there is already a save file.
        MKPlayerData* playerData = MKGameDataLoader::GetInstance()->GetGameData<MKPlayerData>();
        mkString writablePath = playerData->GetWritablePath();
        if (playerData->LoadData(playerData->GetWritablePath()))
        {
            CCLOG("Existing player save found.");
        }
        else
        {
            CCLOG("No existing player save found or save file corrupted. Creating new save.");

            // If there is not save file yet, load the default and save it.
            MK_ASSERT((playerData->LoadData(playerData->GetCachedPath())));
            MK_ASSERT((playerData->SaveData(playerData->GetWritablePath())));
        }
    }
}

void StartScreenScene::InitialiseBackground()
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

void StartScreenScene::InitialiseLogo()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    float halfWidth = visibleSize.width * 0.5f;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float estimatedHalfPoint = origin.x + halfWidth;
    
    Size desiredSize(visibleSize.height * 0.75f, visibleSize.height * 0.375f);
    m_Logo = MKSprite::Create("Textures/UI/Logo.png", false);
    m_Logo->setScaleX(desiredSize.width / m_Logo->getContentSize().width);
    m_Logo->setScaleY(desiredSize.height / m_Logo->getContentSize().height);
    m_Logo->setAnchorPoint(Vec2(0.5f, 1.0f));
    m_Logo->setPosition(Vec2(estimatedHalfPoint, visibleSize.height * 0.8f));

    this->addChild(m_Logo);
}

void StartScreenScene::InitialiseStartLabel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float halfWidth = visibleSize.width * 0.5f;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float estimatedHalfPoint = origin.x + halfWidth;
    
    float fontSize = visibleSize.height * 0.05f;
    m_StartLabel = cocos2d::Label::createWithTTF("Touch screen to start.", "fonts/Marker_Felt.ttf", fontSize);
    m_StartLabel->setPosition(Vec2(estimatedHalfPoint, visibleSize.height * 0.25f));
    m_StartLabel->setTextColor(Color4B::BLACK);

    // add the label as a child to this layer
    this->addChild(m_StartLabel);
}

void StartScreenScene::OnClick(EventCustom * _event)
{
    MKInputClick* input = static_cast<MKInputClick*>(_event->getUserData());

    if (input->m_InputName == START_GAME)
    {
        if (input->m_ButtonState == MKInputClick::ButtonState::RELEASE)
        {
            DeinitialiseInput();
            MKSceneManager::GetInstance()->ReplaceScene("MainMenuScene");
        }
    }
}

#ifndef WIN32
#ifdef SDKBOX_ENABLED
void StartScreenScene::onLogin(bool isLogin, const std::string& msg)
{
    //CCLOG("FB login: %u", isLogin);
    //if (isLogin)
    //{
    //    bool needPermissionForShare = GTFacebookHelper::CheckForPermissionsNeeded(ALL_PUBLISH_PERMISSIONS);
    //    if (needPermissionForShare)
    //        sdkbox::PluginFacebook::requestPublishPermissions(ALL_PUBLISH_PERMISSIONS);
    //    needPermissionForShare = GTFacebookHelper::CheckForPermissionsNeeded(ALL_READ_PERMISSIONS);
    //    if (needPermissionForShare)
    //        sdkbox::PluginFacebook::requestReadPermissions(ALL_READ_PERMISSIONS);
    //}
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
#endif
#endif
