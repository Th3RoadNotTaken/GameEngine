#include "ActionHandler.h"
#include "ScopeWriter.h"
#include "regex"

namespace Fiea::Engine::Content
{
    bool ActionHandler::EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
    {
        if (!value.isObject())
            return false;

        ScopeWriter* sw = writer.As<ScopeWriter>();
        if (sw == nullptr)
            return false;

        if (value.isMember(std::string("type")))
        {
            const std::string type = value[std::string("type")].asString();
            if (type != std::string("Action"))
                return false;

            std::regex pattern(R"(<(\w+)>(\w+))");
            std::smatch matches;
            if (std::regex_match(key, matches, pattern))
            {
                sw->AddActionObject(matches[2], type);
            }
            else
            {
                sw->AddActionObject(key, type);
            }
            return true;
        }

        std::regex pattern(R"(<(Action\w*)>(\w+))");
        std::smatch matches;

        if (std::regex_match(key, matches, pattern))
        {
            sw->AddActionObject(matches[2], matches[1]);
        }
        else
        {
            return false;
        }

        return true;
    }

    bool ActionHandler::ExitKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
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