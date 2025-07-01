#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/ContentService.h"

namespace Fiea::Engine::Content
{
    class ActionListHandler : public IParseHandler
    {
        FIEA_HANDLER_INTERFACE

    public:
        virtual bool EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value) override;
        virtual bool ExitKey(ParseWriter& writer, const std::string& key, const Json::Value& value) override;
    };
}