#pragma once
#include "FIEAGameEngine/Types.h"
#include "FIEAGameEngine/ContentService.h"
#include <iostream>
#include <vector>

namespace Fiea::Engine::Content
{
    class Parser final
    {
    public:
        explicit Parser(const std::vector<std::unique_ptr<IParseHandler>>& parseHandlers);
        void Deserialize(const std::string& jsonString, ParseWriter& writer);
        void Deserialize(std::istream& jsonStream, ParseWriter& writer);
        void DeserializeFieldFromFile(const std::string& filePath, ParseWriter& writer);

    private:
        void ParseField(const std::string& key, const Json::Value& value, ParseWriter& writer);
        void ParseObject(const Json::Value& obj, ParseWriter& writer);
        void ParseArray(const std::string& key, const Json::Value& arr, ParseWriter& writer);

        const std::vector<std::unique_ptr<IParseHandler>>& _ParseHandlers;
    };
}