#include "TestWriter.h"

namespace Fiea::Engine::Content
{
    void TestWriter::RecordInt()
    {
        size_t depth = Depth();

        if (_DataRecord.size() < Depth() + 1)
        {
            _DataRecord.resize(depth + 1, { 0, 0, 0 });
        }
        ++_DataRecord[Depth()].IntCount;
    }

    void TestWriter::RecordFloat()
    {
        size_t depth = Depth();

        if (_DataRecord.size() < Depth() + 1)
        {
            _DataRecord.resize(depth + 1, { 0, 0, 0 });
        }
        ++_DataRecord[Depth()].FloatCount;
    }

    void TestWriter::RecordString()
    {
        size_t depth = Depth();

        if (_DataRecord.size() < Depth() + 1)
        {
            _DataRecord.resize(depth + 1, { 0, 0, 0 });
        }
        ++_DataRecord[Depth()].StringCount;
    }

    const size_t TestWriter::CountInts(const size_t& depth) const
    {
        FIEA_ERROR(_DataRecord.size() > depth);

        return _DataRecord[depth].IntCount;
    }

    const size_t TestWriter::CountFloats(const size_t& depth) const
    {
        FIEA_ERROR(_DataRecord.size() > depth);

        return _DataRecord[depth].FloatCount;
    }

    const size_t TestWriter::CountStrings(const size_t& depth) const
    {
        FIEA_ERROR(_DataRecord.size() > depth);

        return _DataRecord[depth].StringCount;
    }

    void TestWriter::CheckIfValidArray(const std::string& key, const Json::Value& value)
    {
        if (_LastKey == key)
        {
            FIEA_ERROR(value.type() == _LastKeyType);
        }
        _LastKey = key;
        _LastKeyType = value.type();
    }
}