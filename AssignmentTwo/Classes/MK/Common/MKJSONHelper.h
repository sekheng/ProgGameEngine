#ifndef MK_JSON_HELPER_H
#define MK_JSON_HELPER_H

// Include MK
#include "MKMacros.h"

// Include STL
#include <fstream>

// Include JSON
#include "external/json/stringbuffer.h"
#include "external/json/writer.h"
#include "external/json/document.h"

using namespace RAPIDJSON_NAMESPACE;

NS_MK_BEGIN

class MKJSONHelper
{
public:
    static mkString JsonToString(RAPIDJSON_NAMESPACE::Document& _jsonObject)
    {
        RAPIDJSON_NAMESPACE::StringBuffer buffer;
        RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> jsonWriter(buffer);
        _jsonObject.Accept(jsonWriter);

        return buffer.GetString();
    }

    static bool JsonToFile(RAPIDJSON_NAMESPACE::Document& _jsonObject, const mkString& _filePath)
    {
        std::ofstream outputFile;
        outputFile.open(_filePath);
        if (!outputFile.is_open()) { return false; }

        std::string jsonObjectData = JsonToString(_jsonObject);
        outputFile << jsonObjectData;

        outputFile.close();

        return true;
    }
};

NS_MK_END

#endif // MK_JSON_HELPER_H