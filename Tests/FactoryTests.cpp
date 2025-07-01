#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "FIEAGameEngine/FactoryService.h"
#include "Content/Scope.h"
#include "Support/AttributeBox.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;

namespace Fiea::Engine::Tests
{
    using namespace Fiea::Engine::Content;

    TEST_CLASS(FactoryTests)
    {
        TEST_MEMCHECK;

        TEST_METHOD(SimpleScopeFactory)
        {
            ScopeFactory sf;
            Scope* s = sf.Create();

            Datum& d = s->Append("intTest");
            d.Push(20);

            Assert::AreEqual(20, s->Find(string("intTest"))->GetAsInt(0));
            Assert::AreEqual(std::string("Scope"), sf.GetProductName());

            delete s;
        }

        TEST_METHOD(FactoryService)
        {
            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto scopeFactory = std::make_unique<ScopeFactory>();
            fs->Register<Scope>(std::move(scopeFactory));

            Scope* createdScope = fs->Create<Scope>(std::string("Scope"));

            Assert::IsNotNull(createdScope);

            delete createdScope;
            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(AttributeBox)
        {
            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));

            Fiea::Engine::Content::Scope* s = fs->Create<Fiea::Engine::Content::Scope>(std::string("AttributeBox"));

            Assert::IsNotNull(s);

            Fiea::Engine::Tests::Support::AttributeBox* ab = s->As<Fiea::Engine::Tests::Support::AttributeBox>();
            Assert::IsNotNull(ab);

            delete s;
            ServiceManager::ResetServiceMgr();
        }
    };
}