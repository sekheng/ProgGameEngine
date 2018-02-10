#include "GTFacebookHelper.h"
#include "cocos2d.h"
USING_NS_GT

static GTFacebookHelper *m_Instance = nullptr;

GTFacebookHelper::GTFacebookHelper()
{
    CCLOG("Something");
}

GTFacebookHelper::~GTFacebookHelper()
{

}

void GTFacebookHelper::Init()
{
    m_Instance = new GTFacebookHelper();
#ifndef WIN32
#ifdef SDKBOX_ENABLED
    sdkbox::PluginFacebook::setListener(this);
#endif
#endif
}

void GTFacebookHelper::Destroy()
{
    delete m_Instance;
}

#ifndef WIN32
#ifdef SDKBOX_ENABLED
std::string GTFacebookHelper::GetFbName()
{
    return m_Instance->m_FBName;
}

void GTFacebookHelper::onLogin(bool isLogin, const std::string& msg)
{
    if (isLogin)
    {
        bool needPermissionForShare = CheckForPermissionsNeeded(ALL_PUBLISH_PERMISSIONS);
        if (needPermissionForShare)
            sdkbox::PluginFacebook::requestPublishPermissions(ALL_PUBLISH_PERMISSIONS);
        needPermissionForShare = CheckForPermissionsNeeded(ALL_READ_PERMISSIONS);
        if (needPermissionForShare)
            sdkbox::PluginFacebook::requestReadPermissions(ALL_READ_PERMISSIONS);
    }
}
void GTFacebookHelper::onSharedSuccess(const std::string& message)
{

}
void GTFacebookHelper::onSharedFailed(const std::string& message)
{

}
void GTFacebookHelper::onSharedCancel()
{

}
void GTFacebookHelper::onAPI(const std::string& key, const std::string& jsonData)
{
    CCLOG("On API");
}
void GTFacebookHelper::onPermission(bool isLogin, const std::string& msg)
{
    if (isLogin)
    {
        bool needPermissionForShare = CheckForPermissionsNeeded(ALL_READ_PERMISSIONS);
        if (!needPermissionForShare)
        {
            sdkbox::FBAPIParam params;
            params["fields"] = "name, email";
            sdkbox::PluginFacebook::api("me", "GET", params, "me");
        }
    }
}
void GTFacebookHelper::onFetchFriends(bool ok, const std::string& msg)
{

}
void GTFacebookHelper::onRequestInvitableFriends(const sdkbox::FBInvitableFriendsInfo& friends)
{

}
void GTFacebookHelper::onInviteFriendsWithInviteIdsResult(bool result, const std::string& msg)
{

}
void GTFacebookHelper::onInviteFriendsResult(bool result, const std::string& msg)
{

}

void GTFacebookHelper::onGetUserInfo(const sdkbox::FBGraphUser& userInfo)
{
    CCLOG("Get User Info");
}
#endif
#endif
