#pragma once
#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/ContentService.h"

namespace Fiea::Engine::Content
{
    class TestWriter : public ParseWriter
    {
    public:
        void RecordInt();
        void RecordFloat();
        void RecordString();

        const size_t CountInts(const size_t& depth) const;
        const size_t CountFloats(const size_t& depth) const;
        const size_t CountStrings(const size_t& depth) const;

        void CheckIfValidArray(const std::string& key, const Json::Value& value);

    private:
        struct _DataTypeCount
        {
            size_t IntCount;
            size_t FloatCount;
            size_t StringCount;
        };

        std::vector<_DataTypeCount> _DataRecord;

        std::string _LastKey;
        Json::ValueType _LastKeyType;
    };
}