#include "GTServerData.h"
#include "network/HttpClient.h"
#include "external/json/stringbuffer.h"
#include "external/json/writer.h"
#include "external/json/document.h"

using namespace cocos2d::network;
using namespace RAPIDJSON_NAMESPACE;
USING_NS_GT

//#define USING_LOCAL_SERVER

#ifndef USING_LOCAL_SERVER
const static std::string m_ServerAddress("https://runningfromtripleprog.appspot.com/");
#else
const static std::string m_ServerAddress("http://localhost:8080/");
#endif

const std::string GTServerData::GETHIGH_SCORELINK("get_topscore");
const std::string GTServerData::GET10_HIGH_SCORELINK("get_topscore/10");
const std::string GTServerData::POST_SCORE_LINK("store_score");

void GTServerData::PostData(const std::string &_linkData, const std::string &_jsonData, const ccHttpRequestCallback& _funcCallback)
{
    std::string networkLink = m_ServerAddress + _linkData;
    HttpRequest *sendRequest = new HttpRequest();
    std::vector<std::string> headers = { "Content-Type: application/json", "Accept: */*" };
    sendRequest->setUrl(networkLink);
    sendRequest->setHeaders(headers);
    sendRequest->setRequestData(_jsonData.c_str(), _jsonData.length());
    sendRequest->setRequestType(HttpRequest::Type::POST);
    sendRequest->setResponseCallback(_funcCallback);
    HttpClient::getInstance()->send(sendRequest);
    sendRequest->release();
}

void GTServerData::GetData(const std::string &_linkData, const ccHttpRequestCallback& _funcCallback)
{
    std::string networkLink = m_ServerAddress + _linkData;
    HttpRequest *getRequest = new HttpRequest();
    getRequest->setUrl(networkLink);
    getRequest->setRequestType(HttpRequest::Type::GET);
    getRequest->setResponseCallback(_funcCallback);
    HttpClient::getInstance()->send(getRequest);
    getRequest->release();
}

void GTServerData::SendHighScore(const float &_score, const std::string &_name)
{
    // we need to store it in json format!
    Document jsonDoc;
    jsonDoc.SetObject();
    jsonDoc.AddMember("score", _score, jsonDoc.GetAllocator());
    if (_name.length() > 0)
    {
        RAPIDJSON_NAMESPACE::Value nameValue(_name.c_str(), _name.size());
        jsonDoc.AddMember("name", nameValue, jsonDoc.GetAllocator());
    }
    StringBuffer buffer;
    Writer<StringBuffer> jsonWriter(buffer);
    jsonDoc.Accept(jsonWriter);
    GTServerData::PostData(GTServerData::POST_SCORE_LINK, buffer.GetString(), GTServerData::WaitForResponse);
}

void GTServerData::WaitForResponse(HttpClient* _client, HttpResponse* _response)
{
    CCLOG("Sending of Data is %s", _response->isSucceed() ? "Success" : "Failed");
}