#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "Support/AttributeBox.h"
#include "Support/AttributeBoxPlus.h"
#include "FIEAGameEngine/ServiceMgr.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(AttributedTestForAttributeBox)
    {
        TEST_MEMCHECK;

        TEST_METHOD(TestVarBindings)
        {
            AttributeBox ab;

            ab.ExternalInteger = 10;
            Assert::AreEqual(10, ab.Find(string("DefIntVal"))->GetAsInt(0));
            ab.Find(string("DefIntVal"))->Set(50);
            Assert::AreEqual(50, ab.ExternalInteger);

            ab.ExternalFloat = 10.5f;
            Assert::AreEqual(10.5f, ab.Find(string("DefFloatVal"))->GetAsFloat(0));
            ab.Find(string("DefFloatVal"))->Set(12.5f);
            Assert::AreEqual(12.5f, ab.ExternalFloat);

            ab.ExternalString = string("Hi");
            Assert::AreEqual(string("Hi"), ab.Find(string("DefStrVal"))->GetAsString(0));
            ab.Find(string("DefStrVal"))->Set(string("test"));
            Assert::AreEqual(string("test"), ab.ExternalString);

            Foo* newFoo = new Foo(20);
            ab.ExternalPointer = newFoo;
            Assert::AreEqual(20, static_cast<Foo*>(ab.Find(string("DefFooVal"))->GetAsPointer(0))->Data());
            Foo* updatedFoo = new Foo(30);
            ab.Find(string("DefFooVal"))->Set(updatedFoo);
            Assert::AreEqual(30, ab.ExternalPointer->Data());

            delete newFoo;
            delete updatedFoo;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TestArrBindings)
        {
            AttributeBox ab;

            ab.ExternalIntegerArray[0] = 100;
            ab.ExternalIntegerArray[1] = 200;

            Datum* intArrDatum = ab.Find("DefIntArrVal");
            Assert::AreEqual(100, intArrDatum->GetAsInt(0));
            Assert::AreEqual(200, intArrDatum->GetAsInt(1));

            intArrDatum->Set(300, 1);
            Assert::AreEqual(300, ab.ExternalIntegerArray[1]);

            ab.ExternalStringArray[0] = string("Hello");
            ab.ExternalStringArray[1] = string("Test");

            Datum* strArrDatum = ab.Find("DefStrArrVal");
            Assert::AreEqual(string("Test"), strArrDatum->GetAsString(1));

            strArrDatum->Set(string("World"), 1);
            Assert::AreEqual(string("World"), ab.ExternalStringArray[1]);

            ab.ExternalFloatArray[0] = 3.14f;
            Datum* floatArrDatum = ab.Find("DefFloatArrVal");
            Assert::AreEqual(3.14f, floatArrDatum->GetAsFloat(0));

            floatArrDatum->Set(6.28f, 0);
            Assert::AreEqual(6.28f, ab.ExternalFloatArray[0]);

            Foo* foo1 = new Foo(1);
            ab.ExternalPointerArray[0] = foo1;

            Datum* fooArrDatum = ab.Find("DefFooArrVal");
            Foo* retrievedFoo = static_cast<Foo*>(fooArrDatum->GetAsPointer(0));

            Assert::AreEqual(1, retrievedFoo->Data());

            Foo* foo2 = new Foo(2);
            fooArrDatum->Set(foo2, 0);

            Assert::AreEqual(2, ab.ExternalPointerArray[0]->Data());

            delete foo1;
            delete foo2;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(CopyConstructorBindings)
        {
            AttributeBox ab1;

            AttributeBox ab2(ab1);

            ab2.ExternalInteger = 10;
            Assert::AreEqual(10, ab2.Find(string("DefIntVal"))->GetAsInt(0));
            ab2.Find(string("DefIntVal"))->Set(50);
            Assert::AreEqual(50, ab2.ExternalInteger);

            ab1.Find(string("DefIntVal"))->Set(50);

            Assert::AreEqual(ab1.Find(string("DefIntVal"))->GetAsInt(0), ab2.Find(string("DefIntVal"))->GetAsInt(0));
            Assert::AreEqual(ab1.ExternalInteger, ab2.ExternalInteger);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(CopyConstructorWithVals)
        {
            AttributeBox ab1;

            ab1.ExternalInteger = 20;
            ab1.ExternalFloat = 20.5f;
            ab1.ExternalString = std::string("Hello");
            Foo* f0 = new Foo(42);
            Foo* f1 = new Foo(0);
            Foo* f2 = new Foo(10);
            Foo* f3 = new Foo(20);
            Foo* f4 = new Foo(30);
            Foo* f5 = new Foo(40);
            ab1.ExternalPointer = f0;

            for (size_t i = 0; i < 5; i++)
            {
                ab1.ExternalIntegerArray[i] = static_cast<int>(i);
                ab1.ExternalFloatArray[i] = 1.5f * static_cast<float>(i);
                ab1.ExternalStringArray[i] = "String" + std::to_string(i);
            }
            ab1.ExternalPointerArray[0] = f1;
            ab1.ExternalPointerArray[1] = f2;
            ab1.ExternalPointerArray[2] = f3;
            ab1.ExternalPointerArray[3] = f4;
            ab1.ExternalPointerArray[4] = f5;

            AttributeBox ab2(ab1);

            Assert::AreEqual(20, ab2.ExternalInteger);
            Assert::AreEqual(20.5f, ab2.ExternalFloat);
            Assert::AreEqual(string("Hello"), ab2.ExternalString);
            Assert::AreEqual(42, ab2.ExternalPointer->Data());

            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(ab1.ExternalIntegerArray[i], ab2.ExternalIntegerArray[i]);
                Assert::AreEqual(ab1.ExternalFloatArray[i], ab2.ExternalFloatArray[i]);
                Assert::AreEqual(ab1.ExternalStringArray[i], ab2.ExternalStringArray[i]);
                Assert::AreEqual(ab1.ExternalPointerArray[i]->Data(), ab2.ExternalPointerArray[i]->Data());
            }

            delete f0;
            delete f1;
            delete f2;
            delete f3;
            delete f4;
            delete f5;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(CopyAssignment)
        {
            AttributeBox ab1;

            ab1.ExternalInteger = 20;
            ab1.ExternalFloat = 20.5f;
            ab1.ExternalString = std::string("Hello");
            Foo* f0 = new Foo(42);
            Foo* f1 = new Foo(0);
            Foo* f2 = new Foo(10);
            Foo* f3 = new Foo(20);
            Foo* f4 = new Foo(30);
            Foo* f5 = new Foo(40);
            ab1.ExternalPointer = f0;

            for (size_t i = 0; i < 5; i++)
            {
                ab1.ExternalIntegerArray[i] = static_cast<int>(i);
                ab1.ExternalFloatArray[i] = 1.5f * static_cast<float>(i);
                ab1.ExternalStringArray[i] = "String" + std::to_string(i);
            }
            ab1.ExternalPointerArray[0] = f1;
            ab1.ExternalPointerArray[1] = f2;
            ab1.ExternalPointerArray[2] = f3;
            ab1.ExternalPointerArray[3] = f4;
            ab1.ExternalPointerArray[4] = f5;

            AttributeBox ab2;
            ab2 = ab1;

            Assert::AreEqual(20, ab2.ExternalInteger);
            Assert::AreEqual(20.5f, ab2.ExternalFloat);
            Assert::AreEqual(string("Hello"), ab2.ExternalString);
            Assert::AreEqual(42, ab2.ExternalPointer->Data());

            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(ab1.ExternalIntegerArray[i], ab2.ExternalIntegerArray[i]);
                Assert::AreEqual(ab1.ExternalFloatArray[i], ab2.ExternalFloatArray[i]);
                Assert::AreEqual(ab1.ExternalStringArray[i], ab2.ExternalStringArray[i]);
                Assert::AreEqual(ab1.ExternalPointerArray[i]->Data(), ab2.ExternalPointerArray[i]->Data());
            }

            delete f0;
            delete f1;
            delete f2;
            delete f3;
            delete f4;
            delete f5;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(MoveConstructor)
        {
            AttributeBox original;
            original.ExternalInteger = 42;
            original.ExternalString = "Hello";
            original.ExternalFloat = 3.14f;

            AttributeBox movedTo(std::move(original));

            Assert::AreEqual(42, movedTo.ExternalInteger);
            Assert::AreEqual(string("Hello"), movedTo.ExternalString);
            Assert::AreEqual(3.14f, movedTo.ExternalFloat);

            Assert::IsTrue(original.ExternalString.empty());

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(MoveAssignment)
        {
            {
                AttributeBox a;
                a.ExternalInteger = 100;
                a.ExternalString = "MoveTest";
                a.ExternalFloat = 9.81f;

                AttributeBox b;
                b = std::move(a);

                Assert::AreEqual(100, b.ExternalInteger);
                Assert::AreEqual(string("MoveTest"), b.ExternalString);
                Assert::AreEqual(9.81f, b.ExternalFloat);

                Assert::IsTrue(a.ExternalString.empty());
            }

            {
                AttributeBox a;
                a.ExternalInteger = 100;
                a.ExternalString = "MoveTest";
                a.ExternalFloat = 9.81f;

                a = std::move(a);

                Assert::AreEqual(100, a.ExternalInteger);
                Assert::AreEqual(string("MoveTest"), a.ExternalString);
                Assert::AreEqual(9.81f, a.ExternalFloat);
            }

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(Equality)
        {
            AttributeBox ab1;

            ab1.ExternalInteger = 20;
            ab1.ExternalFloat = 20.5f;
            ab1.ExternalString = std::string("Hello");
            Foo* f0 = new Foo(42);
            Foo* f1 = new Foo(0);
            Foo* f2 = new Foo(10);
            Foo* f3 = new Foo(20);
            Foo* f4 = new Foo(30);
            Foo* f5 = new Foo(40);
            ab1.ExternalPointer = f0;

            for (size_t i = 0; i < 5; i++)
            {
                ab1.ExternalIntegerArray[i] = static_cast<int>(i);
                ab1.ExternalFloatArray[i] = 1.5f * static_cast<float>(i);
                ab1.ExternalStringArray[i] = "String" + std::to_string(i);
            }
            ab1.ExternalPointerArray[0] = f1;
            ab1.ExternalPointerArray[1] = f2;
            ab1.ExternalPointerArray[2] = f3;
            ab1.ExternalPointerArray[3] = f4;
            ab1.ExternalPointerArray[4] = f5;

            AttributeBox ab2(ab1);

            Assert::AreEqual(ab1, ab2);

            delete f0;
            delete f1;
            delete f2;
            delete f3;
            delete f4;
            delete f5;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(IsAttribute)
        {
            AttributeBox ab1;

            Assert::IsFalse(ab1.IsAttribute(string("Invalid")));
            Assert::IsTrue(ab1.IsAttribute(string("DefIntVal")));

            ab1.AppendInstanceAttribute(string("Test"));
            Assert::IsFalse(ab1.IsClassAttribute(string("Test")));
            Assert::IsTrue(ab1.IsClassAttribute(string("DefIntVal")));
            Assert::IsTrue(ab1.IsInstanceAttribute(string("Test")));
            Assert::IsFalse(ab1.IsInstanceAttribute(string("Invalid")));

            Assert::IsTrue(ab1.IsClassAttribute(string("ResizableIntArr")));
            Assert::IsFalse(ab1.IsInstanceAttribute(string("ResizableIntArr")));

            Datum* d = ab1.Find(string("ResizableIntArr"));
            d->Push(10);
            d->Push(20);
            Assert::AreEqual(10, ab1.Find(string("ResizableIntArr"))->GetAsInt(0));
            Assert::AreEqual(20, ab1.Find(string("ResizableIntArr"))->GetAsInt(1));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(AttributesVector)
        {
            AttributeBox ab1;
            ab1.AppendInstanceAttribute(string("Test"));

            std::vector<string> allAttributes = ab1.Attributes();
            std::vector<string> instanceAttributes = ab1.InstanceAttributes();
            std::vector<string> classAttributes = ab1.ClassAttributes();

            Assert::IsTrue(std::find(allAttributes.begin(), allAttributes.end(), string("DefIntVal")) != allAttributes.end());
            Assert::IsTrue(std::find(allAttributes.begin(), allAttributes.end(), string("Test")) != allAttributes.end());

            Assert::IsTrue(std::find(instanceAttributes.begin(), instanceAttributes.end(), string("Test")) != instanceAttributes.end());
            Assert::IsTrue(std::find(instanceAttributes.begin(), instanceAttributes.end(), string("DefIntVal")) == instanceAttributes.end());

            Assert::IsTrue(std::find(classAttributes.begin(), classAttributes.end(), string("DefIntVal")) != classAttributes.end());
            Assert::IsTrue(std::find(classAttributes.begin(), classAttributes.end(), string("Test")) == classAttributes.end());

            ServiceManager::ResetServiceMgr();
        }
    };

    TEST_CLASS(AttributedTestForAttributeBoxPlus)
    {
        TEST_MEMCHECK;

        TEST_METHOD(TestVarBindings)
        {
            AttributeBoxPlus ab;

            ab.ExternalInteger = 10;
            Assert::AreEqual(10, ab.Find(string("DefIntVal"))->GetAsInt(0));
            ab.Find(string("DefIntVal"))->Set(50);
            Assert::AreEqual(50, ab.ExternalInteger);

            ab.ExternalFloat = 10.5f;
            Assert::AreEqual(10.5f, ab.Find(string("DefFloatVal"))->GetAsFloat(0));
            ab.Find(string("DefFloatVal"))->Set(12.5f);
            Assert::AreEqual(12.5f, ab.ExternalFloat);

            ab.ExternalFloat2 = 11.5f;
            Assert::AreEqual(11.5f, ab.Find(string("DerivedFloatVal"))->GetAsFloat(0));
            ab.Find(string("DerivedFloatVal"))->Set(13.5f);
            Assert::AreEqual(13.5f, ab.ExternalFloat2);

            ab.ExternalString = string("Hi");
            Assert::AreEqual(string("Hi"), ab.Find(string("DefStrVal"))->GetAsString(0));
            ab.Find(string("DefStrVal"))->Set(string("test"));
            Assert::AreEqual(string("test"), ab.ExternalString);

            Foo* newFoo = new Foo(20);
            ab.ExternalPointer = newFoo;
            Assert::AreEqual(20, static_cast<Foo*>(ab.Find(string("DefFooVal"))->GetAsPointer(0))->Data());
            Foo* updatedFoo = new Foo(30);
            ab.Find(string("DefFooVal"))->Set(updatedFoo);
            Assert::AreEqual(30, ab.ExternalPointer->Data());

            delete newFoo;
            delete updatedFoo;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TestArrBindings)
        {
            AttributeBoxPlus ab;

            ab.ExternalIntegerArray[0] = 100;
            ab.ExternalIntegerArray[1] = 200;

            Datum* intArrDatum = ab.Find("DefIntArrVal");
            Assert::AreEqual(100, intArrDatum->GetAsInt(0));
            Assert::AreEqual(200, intArrDatum->GetAsInt(1));

            intArrDatum->Set(300, 1);
            Assert::AreEqual(300, ab.ExternalIntegerArray[1]);

            ab.ExternalStringArray[0] = string("Hello");
            ab.ExternalStringArray[1] = string("Test");

            Datum* strArrDatum = ab.Find("DefStrArrVal");
            Assert::AreEqual(string("Test"), strArrDatum->GetAsString(1));

            strArrDatum->Set(string("World"), 1);
            Assert::AreEqual(string("World"), ab.ExternalStringArray[1]);

            ab.ExternalFloatArray[0] = 3.14f;
            Datum* floatArrDatum = ab.Find("DefFloatArrVal");
            Assert::AreEqual(3.14f, floatArrDatum->GetAsFloat(0));

            floatArrDatum->Set(6.28f, 0);
            Assert::AreEqual(6.28f, ab.ExternalFloatArray[0]);

            Foo* foo1 = new Foo(1);
            ab.ExternalPointerArray[0] = foo1;

            Datum* fooArrDatum = ab.Find("DefFooArrVal");
            Foo* retrievedFoo = static_cast<Foo*>(fooArrDatum->GetAsPointer(0));

            Assert::AreEqual(1, retrievedFoo->Data());

            Foo* foo2 = new Foo(2);
            fooArrDatum->Set(foo2, 0);

            Assert::AreEqual(2, ab.ExternalPointerArray[0]->Data());

            delete foo1;
            delete foo2;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(CopyConstructorBindings)
        {
            AttributeBoxPlus ab1;

            AttributeBoxPlus ab2(ab1);

            ab2.ExternalInteger = 10;
            Assert::AreEqual(10, ab2.Find(string("DefIntVal"))->GetAsInt(0));
            ab2.Find(string("DefIntVal"))->Set(50);
            Assert::AreEqual(50, ab2.ExternalInteger);

            ab1.Find(string("DefIntVal"))->Set(50);

            Assert::AreEqual(ab1.Find(string("DefIntVal"))->GetAsInt(0), ab2.Find(string("DefIntVal"))->GetAsInt(0));
            Assert::AreEqual(ab1.ExternalInteger, ab2.ExternalInteger);

            ab1.Find(string("DerivedFloatVal"))->Set(12.5f);
            ab2.Find(string("DerivedFloatVal"))->Set(12.5f);

            Assert::AreEqual(ab1.ExternalFloat2, ab2.ExternalFloat2);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(CopyConstructorWithVals)
        {
            AttributeBoxPlus ab1;

            ab1.ExternalInteger = 20;
            ab1.ExternalFloat = 20.5f;
            ab1.ExternalFloat2 = 22.5f;
            ab1.ExternalString = std::string("Hello");
            Foo* f0 = new Foo(42);
            Foo* f1 = new Foo(0);
            Foo* f2 = new Foo(10);
            Foo* f3 = new Foo(20);
            Foo* f4 = new Foo(30);
            Foo* f5 = new Foo(40);
            ab1.ExternalPointer = f0;

            for (size_t i = 0; i < 5; i++)
            {
                ab1.ExternalIntegerArray[i] = static_cast<int>(i);
                ab1.ExternalFloatArray[i] = 1.5f * static_cast<float>(i);
                ab1.ExternalStringArray[i] = "String" + std::to_string(i);
            }
            ab1.ExternalPointerArray[0] = f1;
            ab1.ExternalPointerArray[1] = f2;
            ab1.ExternalPointerArray[2] = f3;
            ab1.ExternalPointerArray[3] = f4;
            ab1.ExternalPointerArray[4] = f5;

            AttributeBoxPlus ab2(ab1);

            Assert::AreEqual(20, ab2.ExternalInteger);
            Assert::AreEqual(20.5f, ab2.ExternalFloat);
            Assert::AreEqual(22.5f, ab2.ExternalFloat2);
            Assert::AreEqual(string("Hello"), ab2.ExternalString);
            Assert::AreEqual(42, ab2.ExternalPointer->Data());

            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(ab1.ExternalIntegerArray[i], ab2.ExternalIntegerArray[i]);
                Assert::AreEqual(ab1.ExternalFloatArray[i], ab2.ExternalFloatArray[i]);
                Assert::AreEqual(ab1.ExternalStringArray[i], ab2.ExternalStringArray[i]);
                Assert::AreEqual(ab1.ExternalPointerArray[i]->Data(), ab2.ExternalPointerArray[i]->Data());
            }

            delete f0;
            delete f1;
            delete f2;
            delete f3;
            delete f4;
            delete f5;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(CopyAssignment)
        {
            AttributeBoxPlus ab1;

            ab1.ExternalInteger = 20;
            ab1.ExternalFloat = 20.5f;
            ab1.ExternalFloat2 = 22.5f;
            ab1.ExternalString = std::string("Hello");
            Foo* f0 = new Foo(42);
            Foo* f1 = new Foo(0);
            Foo* f2 = new Foo(10);
            Foo* f3 = new Foo(20);
            Foo* f4 = new Foo(30);
            Foo* f5 = new Foo(40);
            ab1.ExternalPointer = f0;

            for (size_t i = 0; i < 5; i++)
            {
                ab1.ExternalIntegerArray[i] = static_cast<int>(i);
                ab1.ExternalFloatArray[i] = 1.5f * static_cast<float>(i);
                ab1.ExternalStringArray[i] = "String" + std::to_string(i);
            }
            ab1.ExternalPointerArray[0] = f1;
            ab1.ExternalPointerArray[1] = f2;
            ab1.ExternalPointerArray[2] = f3;
            ab1.ExternalPointerArray[3] = f4;
            ab1.ExternalPointerArray[4] = f5;

            AttributeBoxPlus ab2;
            ab2 = ab1;

            Assert::AreEqual(20, ab2.ExternalInteger);
            Assert::AreEqual(20.5f, ab2.ExternalFloat);
            Assert::AreEqual(22.5f, ab2.ExternalFloat2);
            Assert::AreEqual(string("Hello"), ab2.ExternalString);
            Assert::AreEqual(42, ab2.ExternalPointer->Data());

            for (size_t i = 0; i < 5; i++)
            {
                Assert::AreEqual(ab1.ExternalIntegerArray[i], ab2.ExternalIntegerArray[i]);
                Assert::AreEqual(ab1.ExternalFloatArray[i], ab2.ExternalFloatArray[i]);
                Assert::AreEqual(ab1.ExternalStringArray[i], ab2.ExternalStringArray[i]);
                Assert::AreEqual(ab1.ExternalPointerArray[i]->Data(), ab2.ExternalPointerArray[i]->Data());
            }

            delete f0;
            delete f1;
            delete f2;
            delete f3;
            delete f4;
            delete f5;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(MoveConstructor)
        {
            AttributeBoxPlus original;
            original.ExternalInteger = 42;
            original.ExternalString = "Hello";
            original.ExternalFloat = 3.14f;
            original.ExternalFloat2 = 5.14f;

            AttributeBoxPlus movedTo(std::move(original));

            Assert::AreEqual(42, movedTo.ExternalInteger);
            Assert::AreEqual(string("Hello"), movedTo.ExternalString);
            Assert::AreEqual(3.14f, movedTo.ExternalFloat);
            Assert::AreEqual(5.14f, movedTo.ExternalFloat2);

            Assert::IsTrue(original.ExternalString.empty());

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(MoveAssignment)
        {
            {
                AttributeBoxPlus a;
                a.ExternalInteger = 100;
                a.ExternalString = "MoveTest";
                a.ExternalFloat = 9.81f;
                a.ExternalFloat2 = 11.81f;

                AttributeBoxPlus b;
                b = std::move(a);

                Assert::AreEqual(100, b.ExternalInteger);
                Assert::AreEqual(string("MoveTest"), b.ExternalString);
                Assert::AreEqual(9.81f, b.ExternalFloat);
                Assert::AreEqual(11.81f, b.ExternalFloat2);

                Assert::IsTrue(a.ExternalString.empty());
            }

            {
                AttributeBoxPlus a;
                a.ExternalInteger = 100;
                a.ExternalString = "MoveTest";
                a.ExternalFloat = 9.81f;
                a.ExternalFloat2 = 11.81f;

                a = std::move(a);

                Assert::AreEqual(100, a.ExternalInteger);
                Assert::AreEqual(string("MoveTest"), a.ExternalString);
                Assert::AreEqual(9.81f, a.ExternalFloat);
                Assert::AreEqual(11.81f, a.ExternalFloat2);
            }

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(Equality)
        {
            AttributeBoxPlus ab1;

            ab1.ExternalInteger = 20;
            ab1.ExternalFloat = 20.5f;
            ab1.ExternalFloat = 202.5f;
            ab1.ExternalString = std::string("Hello");
            Foo* f0 = new Foo(42);
            Foo* f1 = new Foo(0);
            Foo* f2 = new Foo(10);
            Foo* f3 = new Foo(20);
            Foo* f4 = new Foo(30);
            Foo* f5 = new Foo(40);
            ab1.ExternalPointer = f0;

            for (size_t i = 0; i < 5; i++)
            {
                ab1.ExternalIntegerArray[i] = static_cast<int>(i);
                ab1.ExternalFloatArray[i] = 1.5f * static_cast<float>(i);
                ab1.ExternalStringArray[i] = "String" + std::to_string(i);
            }
            ab1.ExternalPointerArray[0] = f1;
            ab1.ExternalPointerArray[1] = f2;
            ab1.ExternalPointerArray[2] = f3;
            ab1.ExternalPointerArray[3] = f4;
            ab1.ExternalPointerArray[4] = f5;

            AttributeBoxPlus ab2(ab1);

            Assert::AreEqual(ab1, ab2);

            ab2.ExternalFloat2 = 402.2f;
            Assert::AreNotEqual(ab1, ab2);

            delete f0;
            delete f1;
            delete f2;
            delete f3;
            delete f4;
            delete f5;

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(IsAttribute)
        {
            AttributeBoxPlus ab1;

            Assert::IsFalse(ab1.IsAttribute(string("Invalid")));
            Assert::IsTrue(ab1.IsAttribute(string("DerivedFloatVal")));

            ab1.AppendInstanceAttribute(string("Test"));
            Assert::IsFalse(ab1.IsClassAttribute(string("Test")));
            Assert::IsTrue(ab1.IsClassAttribute(string("DerivedFloatVal")));
            Assert::IsTrue(ab1.IsInstanceAttribute(string("Test")));
            Assert::IsFalse(ab1.IsInstanceAttribute(string("Invalid")));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(AttributesVector)
        {
            AttributeBoxPlus ab1;
            ab1.AppendInstanceAttribute(string("Test"));

            std::vector<string> allAttributes = ab1.Attributes();
            std::vector<string> instanceAttributes = ab1.InstanceAttributes();
            std::vector<string> classAttributes = ab1.ClassAttributes();

            Assert::IsTrue(std::find(allAttributes.begin(), allAttributes.end(), string("DerivedFloatVal")) != allAttributes.end());
            Assert::IsTrue(std::find(allAttributes.begin(), allAttributes.end(), string("Test")) != allAttributes.end());

            Assert::IsTrue(std::find(instanceAttributes.begin(), instanceAttributes.end(), string("Test")) != instanceAttributes.end());
            Assert::IsTrue(std::find(instanceAttributes.begin(), instanceAttributes.end(), string("DerivedFloatVal")) == instanceAttributes.end());

            Assert::IsTrue(std::find(classAttributes.begin(), classAttributes.end(), string("DerivedFloatVal")) != classAttributes.end());
            Assert::IsTrue(std::find(classAttributes.begin(), classAttributes.end(), string("Test")) == classAttributes.end());

            ServiceManager::ResetServiceMgr();
        }
    };
}