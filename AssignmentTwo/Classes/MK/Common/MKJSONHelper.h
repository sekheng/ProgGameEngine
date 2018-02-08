#ifndef MK_JSON_HELPER_H
#define MK_JSON_HELPER_H

// Include MK
#include "MKMacros.h"
#include "MKAssertions.h"

// Include STL
#include <fstream>

// Include JSON
#include "external/json/filewritestream.h"
#include "external/json/filereadstream.h"
#include "external/json/stringbuffer.h"
#include "external/json/writer.h"
#include "external/json/document.h"
#include "cocos2d.h"

using namespace RAPIDJSON_NAMESPACE;

NS_MK_BEGIN

class MKJSONHelper
{
public:
    static const mkU16 m_MaxFileCharacters = 65535;

    static mkString JsonToString(RAPIDJSON_NAMESPACE::Document& _jsonObject)
    {
        RAPIDJSON_NAMESPACE::StringBuffer buffer;
        RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> jsonWriter(buffer);
        _jsonObject.Accept(jsonWriter);

        return buffer.GetString();
    }

    static bool JsonToFile(RAPIDJSON_NAMESPACE::Document& _jsonObject, const mkString& _filePath)
    {
        return cocos2d::FileUtils::getInstance()->writeStringToFile(JsonToString(_jsonObject), _filePath);
    }

    static bool LoadFromJSON(RAPIDJSON_NAMESPACE::Document& _document, const mkString& _filePath)
    {
        FILE* inputFile = std::fopen(_filePath.c_str(), "r");
        if (inputFile == nullptr) { return false; }
        
        char buffer[m_MaxFileCharacters]; // The number of characters in the file must not exceed m_MaxFileCharacters.
        RAPIDJSON_NAMESPACE::FileReadStream fileReadStream(inputFile, buffer, sizeof(buffer));
        _document.ParseStream(fileReadStream);

        std::fclose(inputFile);

        return true;
    }
};

NS_MK_END

#endif // MK_JSON_HELPER_H
