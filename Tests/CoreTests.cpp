#include "Tests.h"
#include "Support/Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(CoreTests)
    {
        static int32_t GetInt(int32_t* ptr)
        {
            FIEA_ASSERT(ptr != nullptr);
            return *ptr;
        }

        TEST_METHOD(ValidIntPointer)
        {
            int32_t a = 10;
            int32_t *p = &a;
            Assert::AreEqual(10, *p);
        }
    };
}