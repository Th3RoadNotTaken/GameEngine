#include "ActionListHandler.h"
#include "ScopeWriter.h"
#include "regex"

namespace Fiea::Engine::Content
{
    bool ActionListHandler::EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
    {
        if (!value.isObject())
            return false;

        ScopeWriter* sw = writer.As<ScopeWriter>();
        if (sw == nullptr)
            return false;

        if (value.isMember(std::string("type")))
        {
            const std::string type = value[std::string("type")].asString();
            if (type != std::string("ActionList"))
                return false;

            std::regex pattern(R"(<(\w+)>(\w+))");
            std::smatch matches;
            if (std::regex_match(key, matches, pattern))
            {
                sw->AddActionListObject(matches[2], type);
            }
            else
            {
                sw->AddActionListObject(key, type);
            }
            return true;
        }

        std::regex pattern(R"(<(ActionList\w*)>(\w+))");
        std::smatch matches;

        if (std::regex_match(key, matches, pattern))
        {
            sw->AddActionListObject(matches[2], matches[1]);
        }
        else
        {
            return false;
        }

        return true;
    }

    bool ActionListHandler::ExitKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
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