#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/ContentService.h"
#include "TestWriter.h"

namespace Fiea::Engine::Content
{
    class FloatHandler : public IParseHandler
    {
        FIEA_HANDLER_INTERFACE

    public:
        virtual bool EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value) override
        {
            TestWriter* testWriter = writer.As<TestWriter>();

            if (testWriter != nullptr)
            {
                if (value.type() == Json::ValueType::realValue)
                {
                    testWriter->CheckIfValidArray(key, value);
                    testWriter->RecordFloat();
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