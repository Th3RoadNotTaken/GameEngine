#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "Support/IntHandler.h"
#include "Support/FloatHandler.h"
#include "Support/StringHandler.h"
#include "Support/TestWriter.h"
#include "FIEAGameEngine/ContentService.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;

namespace Fiea::Engine::Tests
{
    using namespace std::string_literals;

    TEST_CLASS(JsonParserTest)
    {
        TEST_MEMCHECK;

        TEST_METHOD(SimpleJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<IntHandler>());
            jc->AddHandler(std::make_unique<FloatHandler>());
            jc->AddHandler(std::make_unique<StringHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            TestWriter tw;

            std::istringstream simple(R"({
                "int":1,
                "float":2.0,
                "str":"abc"
            })"s);

            jp->Deserialize(simple, tw);

            Assert::AreEqual(1_z, tw.CountInts(0));
            Assert::AreEqual(1_z, tw.CountFloats(0));
            Assert::AreEqual(1_z, tw.CountStrings(0));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(NestedJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<IntHandler>());
            jc->AddHandler(std::make_unique<FloatHandler>());
            jc->AddHandler(std::make_unique<StringHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            TestWriter tw;

            std::istringstream nested(R"({
                "int":1,
                "child":{
                    "int":2
                }
            })"s);

            jp->Deserialize(nested, tw);

            Assert::AreEqual(1_z, tw.CountInts(0));
            Assert::AreEqual(0_z, tw.CountFloats(0));
            Assert::AreEqual(0_z, tw.CountStrings(0));

            Assert::AreEqual(1_z, tw.CountInts(1));
            Assert::AreEqual(0_z, tw.CountFloats(0));
            Assert::AreEqual(0_z, tw.CountStrings(0));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(ArrayJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<IntHandler>());
            jc->AddHandler(std::make_unique<FloatHandler>());
            jc->AddHandler(std::make_unique<StringHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            TestWriter tw;

            std::istringstream array(R"({
                "ints":[
                    1,
                    2,
                    3
                ]
            })"s);

            jp->Deserialize(array, tw);

            Assert::AreEqual(3_z, tw.CountInts(1));
            Assert::AreEqual(0_z, tw.CountFloats(0));
            Assert::AreEqual(0_z, tw.CountStrings(0));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(ComplexJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<IntHandler>());
            jc->AddHandler(std::make_unique<FloatHandler>());
            jc->AddHandler(std::make_unique<StringHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            TestWriter tw;

            std::istringstream array(R"({
                "ints":1,
                "child" : {
                    "floats" : [
                        2.5,
                        3.2,
                        {
                            "newChild" : {
                                "strings":"abc"
                            }
                        }
                    ]
                }
            })"s);

            jp->Deserialize(array, tw);

            Assert::AreEqual(1_z, tw.CountInts(0));
            Assert::AreEqual(0_z, tw.CountFloats(0));
            Assert::AreEqual(0_z, tw.CountStrings(0));

            Assert::AreEqual(0_z, tw.CountInts(2));
            Assert::AreEqual(2_z, tw.CountFloats(2));
            Assert::AreEqual(0_z, tw.CountStrings(2));

            Assert::AreEqual(0_z, tw.CountInts(4));
            Assert::AreEqual(0_z, tw.CountFloats(4));
            Assert::AreEqual(1_z, tw.CountStrings(4));

            ServiceManager::ResetServiceMgr();
        }
    };
}