#include "FooManager.h"

namespace Fiea::Engine::Tests::Support
{
    void FooManager::Make(const char* name, int32_t value)
    {
        if (FooHolder.find(name) == FooHolder.end())
        {
            string nameStr = string(name);
            FooHolder.emplace(nameStr, Foo(value));
        }
    }

    int FooManager::Take(const char* name)
    {
        string nameStr = string(name);
        FIEA_ERROR(FooHolder.find(nameStr) != FooHolder.end());

        return FooHolder[nameStr].Data();
    }
}