#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "Support/Foo.h"
#include "Support/Empty.h"
#include "Support/EmptyChild.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Tests::Support;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(RTTITests)
    {
        TEST_MEMCHECK;

        TEST_METHOD(FooIsFunctionality)
        {
            Foo f1(10);
            RTTI* rtti = &f1;

            Assert::IsTrue(rtti->Is(f1.TypeIdClass()));
            Assert::IsFalse(rtti->Is(0));
            Assert::IsTrue(rtti->Is(f1.TypeIdInstance()));
        }

        TEST_METHOD(FooAsFunctionality)
        {
            Foo f1(20);
            RTTI* rtti = &f1;

            Foo* fooPtr = rtti->As<Foo>();
            Assert::IsNotNull(fooPtr);
            Assert::AreEqual(20, fooPtr->Data());

            RTTI* rttiPtr = static_cast<Foo*>(&f1);
            Assert::IsNotNull(rttiPtr);
        }

        TEST_METHOD(EmptyTest)
        {
            Empty* e1 = new Empty();
            EmptyChild* ec1 = new EmptyChild(5);
            Empty* e2 = new EmptyChild(5);
            Foo* f1 = new Foo(15);

            Assert::IsFalse(e1->Is(ec1->TypeIdClass()));
            Assert::IsTrue(ec1->Is(e1->TypeIdClass()));

            Empty* testE1 = ec1->As<Empty>();
            EmptyChild* testEC1 = e1->As<EmptyChild>();
            Assert::IsNotNull(testE1);
            Assert::IsNull(testEC1);

            Assert::IsTrue(e2->Is(ec1->TypeIdClass()));
            Assert::IsTrue(e2->Is(ec1->TypeIdInstance()));
            Assert::IsTrue(e2->Is(e1->TypeIdClass()));
            Assert::IsFalse(e1->Is(e2->TypeIdInstance()));

            Empty* testE2 = f1->As<Empty>();
            Assert::IsNull(testE2);
            EmptyChild* testEC2 = f1->As<EmptyChild>();
            Assert::IsNull(testEC2);

            Assert::IsTrue(ec1->Equals(e2));
            Assert::IsFalse(e1->Equals(e2));

            Assert::AreEqual(std::string("EmptyChild(5)"), ec1->ToString());

            Assert::AreNotEqual(e1->GetValue(), ec1->GetValue());

            delete e1;
            delete ec1;
            delete e2;
            delete f1;
        }
    };
}