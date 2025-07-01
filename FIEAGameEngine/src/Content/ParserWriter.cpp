#include "FIEAGameEngine/ContentService.h"

namespace Fiea::Engine::Content
{
    RTTI_DEFINITIONS(ParseWriter);

    void ParseWriter::IncrementDepth()
    {
        ++_Depth;
    }

    void ParseWriter::DecrementDepth()
    {
        --_Depth;
    }

    size_t ParseWriter::Depth()
    {
        return _Depth;
    }
}