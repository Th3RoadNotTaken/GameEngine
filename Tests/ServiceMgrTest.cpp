#include "FIEAGameEngine/Types.h"
#include "Support/FooManager.h"
#include "Support/NumberAdder.h"
#include "Tests.h"
#include "Support/Foo.h"
#include "FIEAGameEngine/ServiceMgr.h"
#include "FIEAGameEngine/ClockService.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Tests::Support;
using namespace Fiea::Engine::Clock;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(ServiceProviderTest)
    {
        TEST_MEMCHECK;

        TEST_METHOD(FooServiceTest)
        {
            IFooMaker* fooMaker = ServiceManager::ProvideInterface<IFooMaker>();
            fooMaker->Make("varA", 30);
            IFooTaker* fooTaker = ServiceManager::ProvideInterface<IFooTaker>();
            Assert::AreEqual(30, fooTaker->Take("varA"));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(NumberAdderTest)
        {
            INumAdder* numAdder = ServiceManager::ProvideInterface<INumAdder>();
            Assert::AreEqual(10, numAdder->Add(6, 4));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(ClockServiceTest)
        {
            ITimeProvider* timeProvider = ServiceManager::ProvideInterface<ITimeProvider>();
            ITimeUpdater* timeUpdater = ServiceManager::ProvideInterface<ITimeUpdater>();

            uint32_t initialTime = timeProvider->Time();
            uint32_t newTime = initialTime + 10;
            uint32_t returnedOldTime = timeUpdater->Update(newTime);

            Assert::AreEqual(initialTime, returnedOldTime);

            uint32_t updatedTime = timeProvider->Time();
            Assert::AreEqual(newTime, updatedTime);

            ServiceManager::ResetServiceMgr();
        }
    };
}