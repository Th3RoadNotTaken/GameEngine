#pragma once
#include "FIEAGameEngine/Types.h"
#include "FIEAGameEngine/ServiceMgr.h"  
#include "Foo.h"
#include <unordered_map>

namespace Fiea::Engine::Tests::Support
{
    struct IFooMaker
    {
        FIEA_SERVICE_INTERFACE

        virtual void Make(const char* name, int32_t value) = 0;
    };

    struct IFooTaker
    {
        FIEA_SERVICE_INTERFACE                                                                                           

        virtual int Take(const char* name) = 0;
    };

    class FooManager : public IFooMaker, public IFooTaker
    {
    private:
        std::unordered_map<string, Foo> FooHolder;
        FIEA_SERVICE_BIND2(FooManager, IFooMaker, IFooTaker)

    public:
        virtual void Make(const char* name, int32_t value) override;
        virtual int Take(const char* name) override;
    };
}