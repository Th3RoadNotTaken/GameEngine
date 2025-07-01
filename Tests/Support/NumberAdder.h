#pragma once
#include "FIEAGameEngine/Types.h"
#include "FIEAGameEngine/ServiceMgr.h"  

namespace Fiea::Engine::Tests::Support
{
    struct INumAdder
    {
        FIEA_SERVICE_INTERFACE

        virtual int32_t Add(int32_t val1, int32_t val2) = 0;
    };

    class NumberAdder : public INumAdder
    {
    private:
        FIEA_SERVICE_BIND1(NumberAdder, INumAdder);

    public:
        virtual int32_t Add(int32_t val1, int32_t val2) override;
    };
}