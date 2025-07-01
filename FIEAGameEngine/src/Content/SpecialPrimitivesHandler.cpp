#include "SpecialPrimitivesHandler.h"
#include "ScopeWriter.h"
#include "regex"

namespace Fiea::Engine::Content
{
    bool SpecialPrimitivesHandler::EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
    {
        if (!value.isString())
            return false;

        if (key == "type")
            return false;

        ScopeWriter* sw = writer.As<ScopeWriter>();
        if (sw == nullptr)
            return false;

        std::regex vecPattern(R"(^(vec4)\s*\(.*\)$)");
        std::regex matPattern(R"(^(mat4x4)\s*\(.*\)$)");
        std::smatch matches;
        std::string str = value.asString();
        if (std::regex_match(str, matches, vecPattern))
        {
            sw->AddVectorType(key, value);
        }
        else if (std::regex_match(str, matches, matPattern))
        {
            sw->AddMatrixType(key, value);
        }
        else
            return false;

        return true;
    }

    bool SpecialPrimitivesHandler::ExitKey(ParseWriter& writer, const std::string& key, const Json::Value& value)
    {
        return true;
    }
}