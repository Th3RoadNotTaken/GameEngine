#include "ScopeHandler.h"
#include "ScopeWriter.h"
#include <regex>

namespace Fiea::Engine::Content
{
    bool ScopeHandler::EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
    {
        if (!value.isObject())
            return false;

        ScopeWriter* sw = writer.As<ScopeWriter>();
        if (sw == nullptr)
            return false;

        sw->AddScope(key, std::string());
        
        return true;
    }

    bool ScopeHandler::ExitKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
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