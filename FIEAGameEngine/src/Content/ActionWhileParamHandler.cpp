#include "ActionWhileParamHandler.h"
#include "ScopeWriter.h"
#include "regex"

namespace Fiea::Engine::Content
{
    bool ActionWhileParamHandler::EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
    {
        if (!value.isObject())
            return false;

        ScopeWriter* sw = writer.As<ScopeWriter>();
        if (sw == nullptr)
            return false;

        if (value.isMember(std::string("type")))
        {
            const std::string type = value[std::string("type")].asString();
            if (type != std::string("Preamble") && type != std::string("Condition") && type != std::string("Increment"))
                return false;

            std::regex pattern(R"(<(\w+)>(\w+))");
            std::smatch matches;
            if (std::regex_match(key, matches, pattern))
            {
                sw->AddWhileActionParamObject(matches[2], type);
            }
            else
            {
                sw->AddWhileActionParamObject(key, type);
            }
            return true;
        }

        std::regex pattern(R"(<(Action\w*)>(\w+))");
        std::smatch matches;

        if (std::regex_match(key, matches, pattern) && (matches[2] == std::string("Preamble") || matches[2] == std::string("Condition") || matches[2] == std::string("Increment")))
        {
            sw->AddWhileActionParamObject(matches[2], matches[1]);
        }
        else
        {
            return false;
        }

        return true;
    }

    bool ActionWhileParamHandler::ExitKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
    {
        if (!value.isObject())
            return false;

        ScopeWriter* sw = writer.As<ScopeWriter>();
        if (sw == nullptr)
            return false;

        sw->RemoveObject();
        return true;
    }
}