#pragma once

#include <string>
#include "network/HttpRequest.h"
#include "../Common/GTMacros.h"

NS_GT_BEGIN
class GTServerData
{
public:
    const static std::string GETHIGH_SCORELINK;
    const static std::string GET10_HIGH_SCORELINK;
    const static std::string POST_SCORE_LINK;

    /*
    * Post Data to server with the use of JSON
    *
    * @param _linkData which is the link to get to!
    * @param _jsonData which needs to be in JSON format but string!
    * @param _funcCallback when request is send back!
    */
    static void PostData(const std::string &_linkData, const std::string &_jsonData, const cocos2d::network::ccHttpRequestCallback& _funcCallback);

    /*
    * Get Data from server with function callbacks!
    *
    * @param _linkData which is the link to get to!
    * @param _funcCallback when request is send back!
    */
    static void GetData(const std::string &_linkData, const cocos2d::network::ccHttpRequestCallback& _funcCallback);

    static void SendHighScore(const float &_score, const std::string &_name = "");

private:
    static void WaitForResponse(cocos2d::network::HttpClient* _client, cocos2d::network::HttpResponse* _response);
};
NS_GT_END