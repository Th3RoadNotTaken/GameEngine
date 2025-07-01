#include "Parser.h"
#include <fstream>

namespace Fiea::Engine::Content
{
    Parser::Parser(const std::vector<std::unique_ptr<IParseHandler>>& parseHandlers) : _ParseHandlers(parseHandlers)
    {
    }

    void Parser::Deserialize(const std::string& jsonString, ParseWriter& writer)
    {
        Json::CharReaderBuilder builder;
        std::string errs;
        Json::Value root;
        std::istringstream iss(jsonString);

        bool parsingSuccessful = Json::parseFromStream(builder, iss, &root, &errs);
        FIEA_ERROR(parsingSuccessful == true);

        ParseObject(root, writer);
    }

    void Parser::Deserialize(std::istream& jsonStream, ParseWriter& writer)
    {
        Json::CharReaderBuilder builder;
        std::string errs;
        Json::Value root;

        bool parsingSuccessful = Json::parseFromStream(builder, jsonStream, &root, &errs);
        FIEA_ERROR(parsingSuccessful == true);

        ParseObject(root, writer);
    }

    void Parser::DeserializeFieldFromFile(const std::string& filePath, ParseWriter& writer)
    {
        std::ifstream fileStream(filePath);
        FIEA_ERROR(fileStream.is_open());

        Deserialize(fileStream, writer);
    }

    void Parser::ParseObject(const Json::Value& obj, ParseWriter& writer)
    {
        for (const auto& key : obj.getMemberNames())
        {
            const Json::Value& value = obj[key];
            ParseField(key, value, writer);
        }
    }

    void Parser::ParseArray(const std::string& key, const Json::Value& arr, ParseWriter& writer)
    {
        for (Json::ArrayIndex i = 0; i < arr.size(); ++i)
        {
            ParseField(key, arr[i], writer);
        }
    }

    void Parser::ParseField(const std::string& key, const Json::Value& value, ParseWriter& writer)
    {
        IParseHandler* currentHandler = nullptr;

        for (const auto& handler : _ParseHandlers)
        {
            if (handler->EnterKey(writer, key, value) == true)
            {
                currentHandler = handler.get();
                break;
            }
        }

        if (value.isArray())
        {
            writer.IncrementDepth();
            ParseArray(key, value, writer);
            writer.DecrementDepth();
        }
        else if (value.isObject())
        {
            writer.IncrementDepth();
            ParseObject(value, writer);
            writer.DecrementDepth();
        }

        if (currentHandler != nullptr)
        {
            bool exited = currentHandler->ExitKey(writer, key, value);
            FIEA_ERROR(exited == true);
        }
    }
}