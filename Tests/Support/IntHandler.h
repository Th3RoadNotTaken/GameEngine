#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/ContentService.h"
#include "TestWriter.h"

namespace Fiea::Engine::Content
{
    class IntHandler : public IParseHandler
    {
        FIEA_HANDLER_INTERFACE

    public:
        virtual bool EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value) override
        {
            TestWriter* testWriter = writer.As<TestWriter>();

            if (testWriter != nullptr)
            {
                if (value.type() == Json::ValueType::intValue)
                {
                    testWriter->CheckIfValidArray(key, value);
                    testWriter->RecordInt();
                }
                else
                    return false;
            }
            else
                return false;

            return true;
        }

        virtual bool ExitKey(ParseWriter& writer, const std::string& key, const Json::Value& value) override
        {
            return true;
        }
    };
}