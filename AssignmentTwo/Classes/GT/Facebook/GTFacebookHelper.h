#pragma once

#include "../Common/GTMacros.h"

#include <unordered_set>

#ifndef WIN32
#ifdef SDKBOX_ENABLED
#include "PluginFacebook/PluginFacebook.h"
#define ALL_READ_PERMISSIONS { sdkbox::FB_PERM_READ_PUBLIC_PROFILE, sdkbox::FB_PERM_READ_EMAIL, sdkbox::FB_PERM_READ_USER_FRIENDS }
#define ALL_PUBLISH_PERMISSIONS { sdkbox::FB_PERM_PUBLISH_POST }
#endif
#endif

NS_GT_BEGIN

class GTFacebookHelper
#ifndef WIN32
#ifdef SDKBOX_ENABLED
    : public sdkbox::FacebookListener
#endif
#endif
{
public:
    static void Init();
    static void Destroy();
    /*
    * Helps to check for permissions!
    *
    * @return false means no need for such permission since it is already asked!
    */
    static bool CheckForPermissionsNeeded(const std::unordered_set<std::string> &_permissionSets)
    {
#ifndef WIN32
#ifdef SDKBOX_ENABLED
        int numberOfPermissions = _permissionSets.size();
        for (std::vector<std::string>::iterator it = sdkbox::PluginFacebook::getPermissionList().begin(), end = sdkbox::PluginFacebook::getPermissionList().end(); it != end; ++it)
        {
            if (_permissionSets.count(*it))
            {
                --numberOfPermissions;
                // check whether the number of permissions has reaches 0!
                if (numberOfPermissions == 0)
                {
                    return false;
                }
            }
        }
        return true;
#endif
#endif
        return false;
    }
    
#ifndef WIN32
#ifdef SDKBOX_ENABLED
    static void ResetFBName();
    static std::string GetFbName();
    static void SetFBName(const std::string &_name);

    virtual void onLogin(bool isLogin, const std::string& msg);
    virtual void onSharedSuccess(const std::string& message);
    virtual void onSharedFailed(const std::string& message);
    virtual void onSharedCancel();
    virtual void onAPI(const std::string& key, const std::string& jsonData);
    virtual void onPermission(bool isLogin, const std::string& msg);
    virtual void onFetchFriends(bool ok, const std::string& msg);
    virtual void onRequestInvitableFriends(const sdkbox::FBInvitableFriendsInfo& friends);
    virtual void onInviteFriendsWithInviteIdsResult(bool result, const std::string& msg);
    virtual void onInviteFriendsResult(bool result, const std::string& msg);

    virtual void onGetUserInfo(const sdkbox::FBGraphUser& userInfo);

protected:
    std::string m_FBName;
#endif
#endif
protected:
    GTFacebookHelper();
    virtual ~GTFacebookHelper();
    
    static GTFacebookHelper* m_Instance;
};

NS_GT_END
