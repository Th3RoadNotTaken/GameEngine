#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "Support/Foo.h"
#include "Support/FooManager.h"
#include "Support/NumberAdder.h"
#include "Content/Datum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(DatumTests)
    {
        TEST_MEMCHECK;

        TEST_METHOD(BasicConstructors)
        {
            Datum d1;
            Assert::AreEqual(0_z, d1.Capacity());
            Assert::AreEqual(0_z, d1.Size());

            Datum d2(1);
            Assert::AreEqual(1_z, d2.Capacity());
            Assert::AreEqual(1_z, d2.Size());
            Assert::AreEqual(1, d2.GetAsInt(0));

            Datum d3(1.2f);
            Assert::AreEqual(1_z, d3.Capacity());
            Assert::AreEqual(1_z, d3.Size());
            Assert::AreEqual(1.2f, d3.GetAsFloat(0));

            Datum d4("Hi");
            Assert::AreEqual(1_z, d4.Capacity());
            Assert::AreEqual(1_z, d4.Size());
            Assert::AreEqual(string("Hi"), d4.GetAsString(0));

            glm::vec4 v1{ 1.5f,2.5f,3.5f,4.5f };
            Datum d5{ glm::vec4{ 1.5f,2.5f,3.5f,4.5f } };
            Assert::AreEqual(1_z, d5.Capacity());
            Assert::AreEqual(1_z, d5.Size());
            Assert::AreEqual(v1, d5.GetAsVec4(0));

            glm::mat4x4 defaultMat4x4(1.0f);
            Datum d6{ defaultMat4x4 };
            Assert::AreEqual(1_z, d6.Capacity());
            Assert::AreEqual(1_z, d6.Size());
            Assert::AreEqual(defaultMat4x4, d6.GetAsMat4x4(0));
        }

        TEST_METHOD(InializerList)
        {
            Datum d1{ 1,2,3,4 };
            Assert::AreEqual(4_z, d1.Capacity());
            Assert::AreEqual(4_z, d1.Size());
            Assert::AreEqual(4, d1.GetAsInt(3));

            Datum d2{ 1.2f, 1.5f, 2.4f, 8.7f };
            Assert::AreEqual(4_z, d2.Capacity());
            Assert::AreEqual(4_z, d2.Size());
            Assert::AreEqual(8.7f, d2.GetAsFloat(3));

            Datum d3{ "hi", "this", "is", "a", "test" };
            Assert::AreEqual(5_z, d3.Capacity());
            Assert::AreEqual(5_z, d3.Size());
            Assert::AreEqual(string("test"), d3.GetAsString(4));
            Assert::ExpectException<FieaAbortException>([&]() {
                d3.GetAsString(5);
                });

            Datum dVec4({ glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f) });
            Assert::AreEqual(2_z, dVec4.Size());
            Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), dVec4.GetAsVec4(0));
            Assert::AreEqual(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f), dVec4.GetAsVec4(1));
        
            Datum dMat4x4({ glm::mat4x4(1.0f), glm::mat4x4(0.0f) });
            Assert::AreEqual(2_z, dMat4x4.Size());
            Assert::AreEqual(glm::mat4x4(1.0f), dMat4x4.GetAsMat4x4(0));
            Assert::AreEqual(glm::mat4x4(0.0f), dMat4x4.GetAsMat4x4(1));
        }

        TEST_METHOD(CopyConstructor)
        {
            Datum d1{ 1,2,3,4 };
            Datum d2(d1);

            Assert::IsFalse(&d1 == &d2);

            Assert::AreEqual(d1.Capacity(), d2.Capacity());
            Assert::AreEqual(d1.Size(), d2.Size());

            for (size_t i = 0; i < d1.Size(); i++)
            {
                Assert::AreEqual(d1.GetAsInt(i), d2.GetAsInt(i));
            }

            Datum d3{ 1.2f, 1.5f, 2.4f, 8.7f };
            Datum d4(d3);

            Assert::IsFalse(&d3 == &d4);

            Assert::AreEqual(d3.Capacity(), d4.Capacity());
            Assert::AreEqual(d3.Size(), d4.Size());

            for (size_t i = 0; i < d3.Size(); i++)
            {
                Assert::AreEqual(d3.GetAsFloat(i), d4.GetAsFloat(i));
            }

            Datum d5{ "hi", "this", "is", "a", "test" };
            Datum d6(d5);

            Assert::IsFalse(&d5 == &d6);

            Assert::AreEqual(d5.Capacity(), d6.Capacity());
            Assert::AreEqual(d5.Size(), d6.Size());

            for (size_t i = 0; i < d5.Size(); i++)
            {
                Assert::AreEqual(d5.GetAsString(i), d6.GetAsString(i));
            }

            
            {
                Datum d7({ glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f) });
                Datum d8(d7);

                Assert::IsFalse(&d7 == &d8);

                Assert::AreEqual(d7.Capacity(), d8.Capacity());
                Assert::AreEqual(d7.Size(), d8.Size());

                for (size_t i = 0; i < d7.Size(); i++)
                {
                    Assert::AreEqual(d7.GetAsVec4(i), d8.GetAsVec4(i));
                }
            }

            {
                Datum d7({ glm::mat4x4(1.0f), glm::mat4x4(0.0f) });
                Datum d8(d7);

                Assert::IsFalse(&d7 == &d8);

                Assert::AreEqual(d7.Capacity(), d8.Capacity());
                Assert::AreEqual(d7.Size(), d8.Size());

                for (size_t i = 0; i < d7.Size(); i++)
                {
                    Assert::AreEqual(d7.GetAsMat4x4(i), d8.GetAsMat4x4(i));
                }
            }
        }

        TEST_METHOD(CopyAssignment)
        {
            Datum d1{ 1,2,3,4 };
            Datum d2{ 1,2,3 };
            d2 = d1;

            Assert::IsFalse(&d1 == &d2);

            Assert::AreEqual(d1.Capacity(), d2.Capacity());
            Assert::AreEqual(d1.Size(), d2.Size());

            for (size_t i = 0; i < d1.Size(); i++)
            {
                Assert::AreEqual(d1.GetAsInt(i), d2.GetAsInt(i));
            }

            Datum d3{ 1.2f, 1.5f, 2.4f, 8.7f };
            Datum d4{ 1.5f };
            d4 = d3;

            Assert::IsFalse(&d4 == &d3);

            Assert::AreEqual(d4.Capacity(), d3.Capacity());
            Assert::AreEqual(d4.Size(), d3.Size());

            for (size_t i = 0; i < d3.Size(); i++)
            {
                Assert::AreEqual(d4.GetAsFloat(i), d3.GetAsFloat(i));
            }

            Datum d5{ "hi", "this", "is", "a", "test" };
            Datum d6{ "test" };
            d6 = d5;

            Assert::IsFalse(&d6 == &d5);

            Assert::AreEqual(d6.Capacity(), d5.Capacity());
            Assert::AreEqual(d6.Size(), d5.Size());

            for (size_t i = 0; i < d5.Size(); i++)
            {
                Assert::AreEqual(d6.GetAsString(i), d5.GetAsString(i));
            }

            {
                Datum d7({ glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f) });
                Datum d8({ glm::vec4(1.0f, 2.0f, 3.0f, 4.0f) });
                d8 = d7;

                Assert::IsFalse(&d7 == &d8);

                Assert::AreEqual(d7.Capacity(), d8.Capacity());
                Assert::AreEqual(d7.Size(), d8.Size());

                for (size_t i = 0; i < d7.Size(); i++)
                {
                    Assert::AreEqual(d7.GetAsVec4(i), d8.GetAsVec4(i));
                }
            }

            {
                Datum d7({ glm::mat4x4(1.0f), glm::mat4x4(0.0f) });
                Datum d8({ glm::mat4x4(1.0f) });
                d8 = d7;

                Assert::IsFalse(&d7 == &d8);

                Assert::AreEqual(d7.Capacity(), d8.Capacity());
                Assert::AreEqual(d7.Size(), d8.Size());

                for (size_t i = 0; i < d7.Size(); i++)
                {
                    Assert::AreEqual(d7.GetAsMat4x4(i), d8.GetAsMat4x4(i));
                }
            }
        }

        TEST_METHOD(OpEqual)
        {
            Datum d1, d2, d3;
            Datum d4{ 1,2,3,4 };
            Datum d5{ 1.2f, 1.5f, 2.4f, 8.7f };
            Datum d6{ "hi", "this", "is", "a", "test" };

            d1 = 1;
            Assert::AreEqual(1, d1.GetAsInt(0));

            d2 = 1.2f;
            Assert::AreEqual(1.2f, d2.GetAsFloat(0));

            d3 = string("Hi");
            Assert::AreEqual(string("Hi"), d3.GetAsString(0));

            d4 = 3;
            Assert::AreEqual(4_z, d4.Capacity());
            Assert::AreEqual(1_z, d4.Size());
            Assert::AreEqual(3, d4.GetAsInt(0));
            Assert::ExpectException<FieaAbortException>([&]() {
                d4 = 1.2f;
                });

            //string test = "hi";
            //ExpectSEH<FIEA_ABORT_CODE>([&]() {
            //    d4 = test;
            //    });

            d5 = 9.8f;
            Assert::AreEqual(4_z, d5.Capacity());
            Assert::AreEqual(1_z, d5.Size());
            Assert::AreEqual(9.8f, d5.GetAsFloat(0));

            /*ExpectSEH<FIEA_ABORT_CODE>([&]() {
                d5 = string("Hi");
                });*/

            d6 = string("Whoops");
            Assert::AreEqual(5_z, d6.Capacity());
            Assert::AreEqual(1_z, d6.Size());
            Assert::AreEqual(string("Whoops"), d6.GetAsString(0));

            Assert::ExpectException<FieaAbortException>([&]() {
                d6.GetAsInt();
                });
            Assert::ExpectException<FieaAbortException>([&]() {
                d6.GetAsFloat();
                });
            Assert::ExpectException<FieaAbortException>([&]() {
                d5.GetAsString();
                });

            {
                Datum d7({ glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f) });
                Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), d7.GetAsVec4(0));
                Assert::AreEqual(glm::vec4(5.0f, 6.0f, 7.0f, 8.0f), d7.GetAsVec4(1));
            }

            {
                Datum d7({ glm::mat4x4(1.0f), glm::mat4x4(0.0f) });
                Assert::AreEqual(glm::mat4x4(1.0f), d7.GetAsMat4x4(0));
                Assert::AreEqual(glm::mat4x4(0.0f), d7.GetAsMat4x4(1));
            }
        }

        TEST_METHOD(Push)
        {
            Datum d1;

            d1.Push<int32_t>(20);
            Assert::AreEqual(1_z, d1.Capacity());
            Assert::AreEqual(1_z, d1.Size());
            Assert::AreEqual(20, d1.GetAsInt(0));

            d1.Push<int32_t>(40);
            Assert::AreEqual(2_z, d1.Capacity());
            Assert::AreEqual(2_z, d1.Size());
            Assert::AreEqual(40, d1.GetAsInt(d1.Size() - 1));

            Assert::ExpectException<FieaAbortException>([&]() {
                d1.Push<string>(string("Nope"));
                });

            Assert::ExpectException<FieaAbortException>([&]() {
                d1.Push<float>(2.3f);
                });

            Datum d2;

            d2.Push<float>(23.4f);
            Assert::AreEqual(1_z, d2.Capacity());
            Assert::AreEqual(1_z, d2.Size());
            Assert::AreEqual(23.4f, d2.GetAsFloat(0));

            d2.Push<float>(33.2f);
            Assert::AreEqual(2_z, d2.Capacity());
            Assert::AreEqual(2_z, d2.Size());
            Assert::AreEqual(33.2f, d2.GetAsFloat(d2.Size() - 1));

            Assert::ExpectException<FieaAbortException>([&]() {
                d2.Push<int32_t>(2);
                });

            Assert::ExpectException<FieaAbortException>([&]() {
                d2.Push<string>(string("Nope"));
                });

            Datum d3;

            d3.Push<string>(string("Hi"));
            Assert::AreEqual(1_z, d3.Capacity());
            Assert::AreEqual(1_z, d3.Size());
            Assert::AreEqual(string("Hi"), d3.GetAsString(0));

            d3.Push<string>(string("Hello"));
            Assert::AreEqual(2_z, d3.Capacity());
            Assert::AreEqual(2_z, d3.Size());
            Assert::AreEqual(string("Hello"), d3.GetAsString(d3.Size() - 1));

            Assert::ExpectException<FieaAbortException>([&]() {
                d3.Push<int32_t>(2);
                });

            Assert::ExpectException<FieaAbortException>([&]() {
                d3.Push<float>(2.5f);
                });

            Datum d4;
            d4.Push<Vec4>({ glm::vec4(1.0f, 2.0f, 3.0f, 4.0f) });
            d4.Push<Vec4>({ glm::vec4(2.0f) });
            Assert::AreEqual(2_z, d4.Capacity());
            Assert::AreEqual(2_z, d4.Size());
            Assert::AreEqual(glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), d4.GetAsVec4(0));
            Assert::AreEqual(glm::vec4(2.0f), d4.GetAsVec4(1));

            Datum d5;
            d5.Push<Mat4x4>({ glm::mat4x4(1.0f) });
            d5.Push<Mat4x4>({ glm::mat4x4(2.0f) });
            Assert::AreEqual(2_z, d5.Capacity());
            Assert::AreEqual(2_z, d5.Size());
            Assert::AreEqual(glm::mat4x4(1.0f), d5.GetAsMat4x4(0));
            Assert::AreEqual(glm::mat4x4(2.0f), d5.GetAsMat4x4(1));
        }

        TEST_METHOD(Pop)
        {
            Datum d1{ 1,2,3,4 };
            Datum d2;
            Datum d3{ "Hi", "this" , "is", "a", "test" };

            Assert::ExpectException<FieaAbortException>([&]() {
                d2.Pop();
                });

            d1.Pop();
            Assert::AreEqual(3_z, d1.Size());
            Assert::AreEqual(4_z, d1.Capacity());
            Assert::AreEqual(3, d1.GetAsInt(d1.Size() - 1));

            d3.Pop();
            Assert::AreEqual(4_z, d3.Size());
            Assert::AreEqual(5_z, d3.Capacity());
            Assert::AreEqual(string("a"), d3.GetAsString(d3.Size() - 1));
        }

        TEST_METHOD(EqualityCheck)
        {
            Datum d1{ 1,2,3,4 };
            Datum d2{ 1,2,3 };

            Assert::IsFalse(d1 == d2);
            Assert::IsTrue(d1 != d2);
            d2.Push<int32_t>(4);
            Assert::IsTrue(d1 == d2);
            Assert::IsFalse(d1 != d2);

            Datum d3{ 1.2f,2.5f,6.2f };
            Datum d4{ 1.2f,2.5f,6.2f };

            Assert::IsFalse(d3 != d4);
            Assert::IsTrue(d3 == d4);

            Datum d5{ "hi","hello","test" };
            Datum d6{ "hi","hello" };

            Assert::IsTrue(d5 != d6);
            d6.Push<string>("test");
            Assert::IsTrue(d5 == d6);
            Assert::IsFalse(d5 != d6);

            {
                Datum d7({ glm::vec4(1.0f, 2.0f, 3.0f, 4.0f) });
                Datum d8({ glm::vec4(5.0f, 6.0f, 7.0f, 8.0f) });
                Datum d9({ glm::vec4(5.0f, 6.0f, 7.0f, 8.0f) });
                
                Assert::IsTrue(d7 != d8);
                Assert::IsTrue(d8 == d9);
            }

            {
                Datum d7({ glm::mat4x4(1.0f) });
                Datum d8({ glm::mat4x4(2.0f) });
                Datum d9({ glm::mat4x4(2.0f) });

                Assert::IsTrue(d7 != d8);
                Assert::IsTrue(d8 == d9);
            }
        }

        TEST_METHOD(FalseEquality)
        {
            Datum d1{ 1,2,3 };
            Datum d2{ 1,2,4 };

            Assert::IsTrue(d1 != d2);
            Assert::IsFalse(d1 == d2);

            Datum d3{ 1.2f, 2.5f };
            Datum d4{ 1.2f, 2.2f };

            Assert::IsTrue(d3 != d4);
            Assert::IsFalse(d3 == d4);

            Datum d5{ "hi","hello" };
            Datum d6{ "bye", "hello" };

            Assert::IsTrue(d5 != d6);
            Assert::IsFalse(d5 == d6);

            Datum d7, d8;
            Assert::IsTrue(d7 == d8);
            Assert::IsFalse(d7 != d8);
        }

        TEST_METHOD(SetGetAndPushUsingStrings)
        {
            Datum d0;
            Assert::ExpectException<FieaAbortException>([&]() {
                d0.SetFromString(string("Test"), 1);
                });

            Datum d1{ 1,2,3 };
            d1.SetFromString(string("6"), 0);
            Assert::AreEqual(6, d1.GetAsInt(0));
            Assert::AreEqual(3_z, d1.Size());
            Assert::AreEqual(string("2"), d1.GetToString(1));
            d1.PushFromString(string("7"));
            Assert::AreEqual(7, d1.GetAsInt(d1.Size() - 1));

            Datum d2{ 1.2f, 2.5f };
            d2.SetFromString(string("7.8"), 1);
            Assert::AreEqual(7.8f, d2.GetAsFloat(1));
            Assert::AreEqual(2_z, d2.Size());
            Assert::AreEqual(string("1.200000"), d2.GetToString(0));
            d2.PushFromString(string("8.9"));
            Assert::AreEqual(8.9f, d2.GetAsFloat(d2.Size() - 1));

            Datum d3{ "hi", "hello" };
            d3.SetFromString(string("bye"), 1);
            Assert::AreEqual(string("bye"), d3.GetAsString(1));
            Assert::AreEqual(2_z, d3.Size());
            Assert::AreEqual(string("bye"), d3.GetToString(1));
            d3.PushFromString(string("Test"));
            Assert::AreEqual(string("Test"), d3.GetAsString(d3.Size() - 1));

            Assert::ExpectException<FieaAbortException>([&]() {
                d3.SetFromString("test", 3);
                });

            Datum d4;
            Assert::ExpectException<FieaAbortException>([&]() {
                d4.SetFromString("test", 2);
                });
            Assert::ExpectException<FieaAbortException>([&]() {
                d4.GetAsString(0);
                });
            Assert::ExpectException<FieaAbortException>([&]() {
                d4.GetToString(0);
                });
            Assert::ExpectException<FieaAbortException>([&]() {
                d4.PushFromString("1.2");
                });

            {
                Datum d1{ glm::vec4(1.0f, 2.0f, 3.0f, 4.0f), glm::vec4(5.0f, 6.0f, 7.0f, 8.0f) };
                d1.SetFromString("vec4(9.0, 10.0, 11.0, 12.0)", 0);
                Assert::AreEqual(glm::vec4(9.0f, 10.0f, 11.0f, 12.0f), d1.GetAsVec4(0));
                Assert::AreEqual(2_z, d1.Size());
                Assert::AreEqual(string("vec4(5.000000, 6.000000, 7.000000, 8.000000)"), d1.GetToString(1));
                d1.PushFromString("vec4(13.0, 14.0, 15.0, 16.0)");
                Assert::AreEqual(glm::vec4(13.0f, 14.0f, 15.0f, 16.0f), d1.GetAsVec4(d1.Size() - 1));

                Datum d2{ glm::mat4x4(1.0f), glm::mat4x4(2.0f) };
                d2.SetFromString("mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))", 1);
                Assert::AreEqual(glm::mat4x4(1.0f), d2.GetAsMat4x4(1));
                Assert::AreEqual(2_z, d2.Size());
                Assert::AreEqual(string("mat4x4((1.000000, 0.000000, 0.000000, 0.000000), (0.000000, 1.000000, 0.000000, 0.000000), (0.000000, 0.000000, 1.000000, 0.000000), (0.000000, 0.000000, 0.000000, 1.000000))"), d2.GetToString(1));
                d2.PushFromString("mat4x4((3.0, 0.0, 0.0, 0.0), (0.0, 3.0, 0.0, 0.0), (0.0, 0.0, 3.0, 0.0), (0.0, 0.0, 0.0, 3.0))");
                Assert::AreEqual(glm::mat4x4(3.0f), d2.GetAsMat4x4(d2.Size() - 1));

                Datum d3;
                Assert::ExpectException<FieaAbortException>([&]() {
                    d3.SetFromString("vec4(1.0, 2.0, 3.0, 4.0)", 2);
                    });
                Assert::ExpectException<FieaAbortException>([&]() {
                    d3.GetAsVec4(0);
                    });
                Assert::ExpectException<FieaAbortException>([&]() {
                    d3.GetToString(0);
                    });
                Assert::ExpectException<FieaAbortException>([&]() {
                    d3.PushFromString("vec4(1.0, 2.0, 3.0, 4.0)");
                    });

                Datum d4;
                Assert::ExpectException<FieaAbortException>([&]() {
                    d4.SetFromString("mat4x4((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))", 3);
                    });
                Assert::ExpectException<FieaAbortException>([&]() {
                    d4.GetAsMat4x4(0);
                    });
                Assert::ExpectException<FieaAbortException>([&]() {
                    d4.GetToString(0);
                    });
                Assert::ExpectException<FieaAbortException>([&]() {
                    d4.PushFromString("mat4x4((1.0, 0.0, 0.0, 0.0), (0.0, 1.0, 0.0, 0.0), (0.0, 0.0, 1.0, 0.0), (0.0, 0.0, 0.0, 1.0))");
                    });
            }
        }

        TEST_METHOD(ExternalStorage)
        {
            int32_t externalArray[] = { 10, 20, 30 };
            Datum d1;
            d1.SetStorage<int32_t>(externalArray, 3);

            /*Datum d2;
            ExpectSEH<FIEA_ABORT_CODE>([&]() {
                d2 = d1;
                });*/

            Assert::AreEqual(3_z, d1.Capacity());
            Assert::AreEqual(3_z, d1.Size());
            Assert::AreEqual(30, d1.GetAsInt(d1.Size() - 1));

            externalArray[2] = 60;
            Assert::AreEqual(60, d1.GetAsInt(d1.Size() - 1));

            float externalFloatArr[] = { 1.2f, 2.5f };
            Datum d3;
            d3.SetStorage<float>(externalFloatArr, 2);

            Assert::AreEqual(2_z, d3.Capacity());
            Assert::AreEqual(2_z, d3.Size());
            Assert::AreEqual(2.5f, d3.GetAsFloat(d3.Size() - 1));

            externalFloatArr[1] = 5.1f;
            Assert::AreEqual(5.1f, d3.GetAsFloat(d3.Size() - 1));

            string externalStrArr[] = { "hi", "hello", "test" };
            Datum d4;
            d4.SetStorage(externalStrArr, 2);

            Assert::AreEqual(2_z, d4.Capacity());
            Assert::AreEqual(2_z, d4.Size());
            Assert::AreEqual(string("hello"), d4.GetAsString(d4.Size() - 1));
        
            externalStrArr[1] = "nope";
            Assert::AreEqual(string("nope"), d4.GetAsString(d4.Size() - 1));
        
            {
                glm::f32vec4 vecArray[] = {
                glm::f32vec4(1.0f, 2.0f, 3.0f, 4.0f),
                glm::f32vec4(5.0f, 6.0f, 7.0f, 8.0f),
                glm::f32vec4(9.0f, 10.0f, 11.0f, 12.0f)
                };
                Datum d1;
                d1.SetStorage(vecArray, std::size(vecArray));
                Assert::AreEqual(3_z, d1.Size());
                Assert::AreEqual(3_z, d1.Capacity());
                Assert::AreEqual(glm::f32vec4(1.0f, 2.0f, 3.0f, 4.0f), d1.GetAsVec4(0));
                Assert::AreEqual(glm::f32vec4(5.0f, 6.0f, 7.0f, 8.0f), d1.GetAsVec4(1));
                Assert::AreEqual(glm::f32vec4(9.0f, 10.0f, 11.0f, 12.0f), d1.GetAsVec4(2));
            }

            {
                glm::f32mat4x4 matArray[] = {
                glm::f32mat4x4(1.0f),
                glm::f32mat4x4(2.0f),
                glm::f32mat4x4(3.0f)
                };
                Datum d2;
                d2.SetStorage(matArray, std::size(matArray));
                Assert::AreEqual(3_z, d2.Size());
                Assert::AreEqual(3_z, d2.Capacity());
                Assert::AreEqual(glm::f32mat4x4(1.0f), d2.GetAsMat4x4(0));
                Assert::AreEqual(glm::f32mat4x4(2.0f), d2.GetAsMat4x4(1));
                Assert::AreEqual(glm::f32mat4x4(3.0f), d2.GetAsMat4x4(2));

                Assert::ExpectException<FieaAbortException>([&]() {
                    d2.Push(glm::f32mat4x4(4.0f));
                    });
            }
        }

        TEST_METHOD(MoveSemantics)
        {
            Datum d1{ 1,2,3 };
            Datum d2(std::move(d1));

            Assert::AreEqual(3_z, d2.Size());
            Assert::AreEqual(3_z, d2.Capacity());
            Assert::AreEqual(3, d2.GetAsInt(d2.Size() - 1));

            Assert::AreEqual(0_z, d1.Size());
            Assert::AreEqual(0_z, d1.Capacity());

            Datum d3{ "hi","hello","test" };
            Datum d0{ "boom" };
            d0 = std::move(d3);

            Assert::AreEqual(3_z, d0.Size());
            Assert::AreEqual(3_z, d0.Capacity());
            Assert::AreEqual(string("test"), d0.GetAsString(d0.Size() - 1));

            Assert::AreEqual(0_z, d3.Size());
            Assert::AreEqual(0_z, d3.Capacity());

            Datum d4{ 12,50,90 };
            Datum d5{ 1,2 };
            d5 = std::move(d4);
            Assert::AreEqual(3_z, d5.Size());
            Assert::AreEqual(3_z, d5.Capacity());
            Assert::AreEqual(90, d5.GetAsInt(d5.Size() - 1));

            Assert::AreEqual(0_z, d4.Size());
            Assert::AreEqual(0_z, d4.Capacity());

            Datum d6{ 1.2f,5.0f,9.7f };
            Datum d7{ 4.3f,7.1f };
            d7 = std::move(d6);
            Assert::AreEqual(3_z, d7.Size());
            Assert::AreEqual(3_z, d7.Capacity());
            Assert::AreEqual(9.7f, d7.GetAsFloat(d7.Size() - 1));

            Assert::AreEqual(0_z, d6.Size());
            Assert::AreEqual(0_z, d6.Capacity());
        }

        TEST_METHOD(ConstDatums)
        {
            const Datum d1{ 1,2,3 };
            const Datum d2{ 1.2f, 2.5f, 5.2f };
            const Datum d3{ "Hi","Test" };

            Assert::AreEqual(3, d1.GetAsInt(d1.Size() - 1));
            Assert::AreEqual(5.2f, d2.GetAsFloat(d2.Size() - 1));
            Assert::AreEqual(string("Test"), d3.GetAsString(d3.Size() - 1));
        }

        TEST_METHOD(PointerType)
        {
            {
                Foo* f1 = new Foo(10);
                Foo* f2 = new Foo(20);
                Datum d1;

                d1.Push(f1);
                d1.Push(f2);

                RTTI* rtti1 = d1.GetAsPointer(0);
                RTTI* rtt2 = d1.GetAsPointer(1);

                Assert::AreEqual(f1, static_cast<Foo*>(rtti1));

                delete f1;
                delete f2;
            }

            {
                Foo* f1 = new Foo(10);
                Foo* f2 = new Foo(20);

                Datum d1;
                d1.Push(f1);
                d1.Push(f2);

                Datum d2(d1);
                Assert::AreEqual(f1, static_cast<Foo*>(d2.GetAsPointer(0)));
                Assert::AreEqual(f2, static_cast<Foo*>(d2.GetAsPointer(1)));

                Datum d3;
                d3 = d1;
                Assert::AreEqual(f1, static_cast<Foo*>(d3.GetAsPointer(0)));
                Assert::AreEqual(f2, static_cast<Foo*>(d3.GetAsPointer(1)));

                Datum d4;
                d4.Push(f1);
                Assert::AreEqual(d1, d2);
                Assert::AreEqual(d1, d3);
                Assert::AreEqual(d2, d3);
                Assert::AreNotEqual(d1, d4);

                delete f1;
                delete f2;
            }

            {
                Foo* f1 = new Foo(10);
                Foo* f2 = new Foo(20);
                Foo* f3 = new Foo(30);

                Datum d1{ f1,f2 };

                Datum d2{ f1,f3 };

                Assert::IsTrue(d1 != d2);

                delete f1;
                delete f2;
                delete f3;
            }

            {
                Foo* f1 = new Foo(10);

                Datum d1;

                d1 = f1;

                RTTI* rtti1 = d1.GetAsPointer(0);
                Assert::AreEqual(f1, static_cast<Foo*>(rtti1));
                Assert::AreEqual(1_z, d1.Size());

                Foo* f2 = new Foo(30);
                d1 = f2;

                RTTI* rtti2 = d1.GetAsPointer(0);
                Assert::AreEqual(f2, static_cast<Foo*>(rtti2));
                Assert::AreEqual(30, static_cast<Foo*>(rtti2)->Data());

                delete f1;
                delete f2;
            }

            {
                Foo* f1 = new Foo(10);
                Foo* f2 = new Foo(20);
                Foo* f3 = new Foo(30);

                Datum d1;
                d1.Push(f1);

                Datum d2;
                d2.Push(f2);
                d2.Push(f3);

                d2 = d1;

                RTTI* rtti1 = d2.GetAsPointer(0);
                Assert::AreEqual(f1, static_cast<Foo*>(rtti1));

                delete f1;
                delete f2;
                delete f3;
            }

            {
                Foo* f1 = new Foo(10);
                Datum d1(f1);

                RTTI* rtti1 = d1.GetAsPointer(0);
                Assert::AreEqual(f1, static_cast<Foo*>(rtti1));

                delete f1;
            }
        }

        TEST_METHOD(TableType)
        {
            {
                Scope* s1 = new Scope();
                Scope* s2 = new Scope();

                Datum d1;
                d1.Push(s1);
                d1.Push(s2);

                Scope* ret1 = d1.GetAsTable(0);
                Scope* ret2 = d1.GetAsTable(1);

                Assert::AreEqual(s1, ret1);
                Assert::AreEqual(s2, ret2);

                delete s1;
                delete s2;
            }

            {
                Scope* s1 = new Scope();
                Scope* s2 = new Scope();

                Datum d1;
                d1.Push(s1);
                d1.Push(s2);

                Datum d2(d1);

                Scope* ret1 = d2.GetAsTable(0);
                Scope* ret2 = d2.GetAsTable(1);

                Assert::AreEqual(s1, ret1);
                Assert::AreEqual(s2, ret2);

                Datum d3;
                d3 = d1;
                Assert::AreEqual(s1, d3.GetAsTable(0));
                Assert::AreEqual(s2, d3.GetAsTable(1));

                Datum d4;
                Assert::AreEqual(d1, d2);
                Assert::AreEqual(d2, d3);
                Assert::AreEqual(d1, d3);
                Assert::AreNotEqual(d1, d4);

                delete s1;
                delete s2;
            }

            {
                Scope* s1 = new Scope();
                Scope* s2 = new Scope();
                Scope* s3 = new Scope();

                Datum d1{ s1,s2 };

                Datum d2{ s1,s3 };

                Assert::AreEqual(d1, d2);

                delete s1;
                delete s2;
                delete s3;
            }

            {
                Scope* s1 = new Scope();

                Datum d1;

                d1 = s1;

                Scope* ret1 = d1.GetAsTable(0);
                Assert::AreEqual(s1, ret1);
                Assert::AreEqual(1_z, d1.Size());

                Scope* s2 = new Scope();
                d1 = s2;

                Scope* ret2 = d1.GetAsTable(0);
                Assert::AreEqual(s2, ret2);

                delete s1;
                delete s2;
            }

            {
                Scope* s1 = new Scope;
                Scope* s2 = new Scope;
                Scope* s3 = new Scope;

                Datum d1;
                d1.Push(s1);

                Datum d2;
                d2.Push(s2);
                d2.Push(s3);

                d2 = d1;

                Scope* ret1 = d2.GetAsTable(0);
                Assert::AreEqual(s1, ret1);

                delete s1;
                delete s2;
                delete s3;
            }

            {
                Scope* s1 = new Scope;
                Datum d1(s1);

                Scope* ret1 = d1.GetAsTable(0);
                Assert::AreEqual(s1, ret1);

                delete s1;
            }
        }
    };
}