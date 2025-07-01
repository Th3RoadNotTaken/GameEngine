#pragma once
#include "FIEAGameEngine/RTTI.h"

namespace Fiea::Engine::Tests::Support
{
    class Empty : public Fiea::Engine::RTTI
    {
        RTTI_DECLARATIONS(Empty, RTTI);

    public:
        virtual int32_t GetValue() const { return 0; }
    };

    RTTI_DEFINITIONS(Empty);
}