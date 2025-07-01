#include "PrimitiveTypeHandler.h"
#include "ScopeWriter.h"

namespace Fiea::Engine::Content
{
    bool PrimitiveTypeHandler::EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
    {
        if (!value.isInt() && !value.isDouble() && !value.isString())
            return false;

        if (key == "type")
            return false;

        ScopeWriter* sw = writer.As<ScopeWriter>();
        if (sw == nullptr)
            return false;

        sw->AddPrimitiveType(key, value);
        return true;
    }

    bool PrimitiveTypeHandler::ExitKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
    {
        return true;
    }
}