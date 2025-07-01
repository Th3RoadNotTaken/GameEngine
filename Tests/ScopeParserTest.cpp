#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "FIEAGameEngine/ContentService.h"
#include "../src/Content/PrimitiveTypeHandler.h"
#include "../src/Content/ScopeHandler.h"
#include "../src/Content/ScopeWriter.h"
#include "../src/Content/ScopeFactoryHandler.h"
#include "../src/Content/SpecialPrimitivesHandler.h"
#include "../src/Content/GameObjectHandler.h"
#include "../src/Content/ActionHandler.h"
#include "../src/Content/ActionListHandler.h"
#include "../src/Content/ActionWhileParamHandler.h"
#include "Support/AttributeBox.h"
#include "../src/Actions/ActionAddDatum.h"
#include "../src/Actions/ActionIncrement.h"
#include "../src/Actions/ActionDecrement.h"
#include "../src/Actions/ActionListWhileLoop.h"
#include "../src/Actions/ActionPreamble.h"
#include "../src/Actions/ActionCondition.h"
#include "../src/Actions/ActionLoopIncrement.h"
#include "../src/Actions/ActionTimed.h"
#include "../src/Actions/ActionDelayed.h"
#include "Support/ActionOutput.h"
#include "../src/Actions/ActionConditionalIncrement.h"
#include "FIEAGameEngine/ActionList.h"

#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;
using namespace Fiea::Engine::Actions;

namespace Fiea::Engine::Tests
{
    using namespace std::string_literals;

    TEST_CLASS(ScopeParserTest)
    {
        //TEST_MEMCHECK;

        TEST_METHOD(SimpleJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());  

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            std::istringstream simple(R"({
                "int":1,
                "float":2.0,
                "str":"abc"
            })"s);

            jp->Deserialize(simple, sw);

            Scope* topScope = sw.GetTopScope();
            Assert::AreEqual(3_z, topScope->Size());

            Assert::AreEqual(1, topScope->Find(string("int"))->GetAsInt(0));
            Assert::AreEqual(2.0f, topScope->Find(string("float"))->GetAsFloat(0));
            Assert::AreEqual(string("abc"), topScope->Find(string("str"))->GetAsString(0));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(DeserializeFieldFromFile)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            const std::string filePath = "test_file.json";
            std::ofstream outFile(filePath);
            outFile << R"({
                        "int": 1,
                        "float": 2.0,
                        "str": "abc"
                    })";
            outFile.close();

            ScopeWriter sw;
            jp->DeserializeFieldFromFile(filePath, sw);

            Scope* topScope = sw.GetTopScope();
            Assert::AreEqual(3_z, topScope->Size());

            Assert::AreEqual(1, topScope->Find(string("int"))->GetAsInt(0));
            Assert::AreEqual(2.0f, topScope->Find(string("float"))->GetAsFloat(0));
            Assert::AreEqual(string("abc"), topScope->Find(string("str"))->GetAsString(0));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(SimpleArrayJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            {
                ScopeWriter sw;

                std::istringstream simple(R"({
                "intArr": [
                    2,
                    4,
                    6
                    ]
                })"s);

                jp->Deserialize(simple, sw);

                Scope* topScope = sw.GetTopScope();
                Assert::AreEqual(1_z, topScope->Size());

                Datum* d = topScope->Find(string("intArr"));
                Assert::AreEqual(2, d->GetAsInt(0));
                Assert::AreEqual(4, d->GetAsInt(1));
                Assert::AreEqual(6, d->GetAsInt(2));
            }

            {
                ScopeWriter sw;

                std::istringstream simple(R"({
                "floatArr": [
                    2.5,
                    4.5,
                    6.5
                    ]
                })"s);

                jp->Deserialize(simple, sw);

                Scope* topScope = sw.GetTopScope();
                Assert::AreEqual(1_z, topScope->Size());

                Datum* d = topScope->Find(string("floatArr"));
                Assert::AreEqual(2.5f, d->GetAsFloat(0));
                Assert::AreEqual(4.5f, d->GetAsFloat(1));
                Assert::AreEqual(6.5f, d->GetAsFloat(2));
            }

            {
                ScopeWriter sw;

                std::istringstream simple(R"({
                "strArr": [
                    "this",
                    "is",
                    "a",
                    "test"
                    ]
                })"s);

                jp->Deserialize(simple, sw);

                Scope* topScope = sw.GetTopScope();
                Assert::AreEqual(1_z, topScope->Size());

                Datum* d = topScope->Find(string("strArr"));
                Assert::AreEqual(string("this"), d->GetAsString(0));
                Assert::AreEqual(string("is"), d->GetAsString(1));
                Assert::AreEqual(string("a"), d->GetAsString(2));
                Assert::AreEqual(string("test"), d->GetAsString(3));
            }

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(InvalidArrayJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            {
                ScopeWriter sw;

                std::istringstream simple(R"({
                "strArr": [
                    "this",
                    "is",
                    "a",
                    2
                    ]
                })"s);

                Assert::ExpectException<FieaAbortException>([&]() {
                    jp->Deserialize(simple, sw);
                    });
            }

            {
                ScopeWriter sw;

                std::istringstream simple(R"({
                "strArr": [
                    "this",
                    "is",
                    "a",
                    2.5
                    ]
                })"s);

                Assert::ExpectException<FieaAbortException>([&]() {
                    jp->Deserialize(simple, sw);
                    });
            }

            {
                ScopeWriter sw;

                std::istringstream simple(R"({
                "strArr": [
                    "this",
                    "is",
                    "a",
                    "newObj" : {
                    "a" : 5
                    }
                    ]
                })"s);

                Assert::ExpectException<FieaAbortException>([&]() {
                    jp->Deserialize(simple, sw);
                    });
            }

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(NestedJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            std::istringstream nested(R"({
                "int":1,
                "child":{
                    "int":2
                }
            })"s);

            jp->Deserialize(nested, sw);

            Scope* topScope = sw.GetTopScope();
            Assert::AreEqual(1, topScope->Find(string("int"))->GetAsInt(0));

            Datum* childDatum = topScope->Find(string("child"));
            Scope* childScope = childDatum->GetAsTable(0);

            Assert::AreEqual(2, childScope->Find("int")->GetAsInt(0));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(ComplexJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            std::istringstream complex(R"({
                "ints":1,
                "child" : {
                    "childArr" : [
                        {
                            "newChild1" : {
                            "string1":"abc"
                            }
                        },
                        {
                            "newChild2" : {
                            "int1":5
                            }
                        },
                        {
                            "newChild3" : {
                            "float1":5.5
                            }    
                        },
                        {
                            "newChild4" : {
                                "newChild5" : {
                                    "int2" : 10
                                }
                            }
                        }
                    ]
                }
            })"s);

            jp->Deserialize(complex, sw);

            Scope* topScope = sw.GetTopScope();
            Assert::AreEqual(1, topScope->Find(string("ints"))->GetAsInt(0));

            Datum* outerChildDatum = topScope->Find(string("child"));
            Scope* outerChildScope = outerChildDatum->GetAsTable(0);

            Datum* arrDatum = outerChildScope->Find(string("childArr"));
            Scope* child1 = arrDatum->GetAsTable(0);
            Scope* child2 = arrDatum->GetAsTable(1);
            Scope* child3 = arrDatum->GetAsTable(2);
            Scope* child4 = arrDatum->GetAsTable(3);

            {
                Datum* d = child1->Find(string("newChild1"));
                Assert::AreEqual(string("abc"), d->GetAsTable(0)->Find(string("string1"))->GetAsString(0));
            }

            {
                Datum* d = child2->Find(string("newChild2"));
                Assert::AreEqual(5, d->GetAsTable(0)->Find(string("int1"))->GetAsInt(0));
            }

            {
                Datum* d = child3->Find(string("newChild3"));
                Assert::AreEqual(5.5f, d->GetAsTable(0)->Find(string("float1"))->GetAsFloat(0));
            }

            {
                Datum* d = child4->Find(string("newChild4"));
                Assert::AreEqual(10, d->GetAsTable(0)->Find(string("newChild5"))->GetAsTable(0)->Find(string("int2"))->GetAsInt(0));
            }

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(SimpleAttributeBox)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            std::istringstream nested(R"({
                "int":1,
                "<AttributeBox>child":{
                    "int":2,
                    "DefIntVal":5
                }
            })"s);

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));

            jp->Deserialize(nested, sw);

            Scope* topScope = sw.GetTopScope();

            Assert::AreEqual(1, topScope->Find(string("int"))->GetAsInt(0));

            Datum* childDatum = topScope->Find(string("child"));
            Scope* childScope = childDatum->GetAsTable(0);

            AttributeBox* ab = childScope->As<AttributeBox>();

            Assert::AreEqual(2, ab->Find("int")->GetAsInt(0));
            Assert::AreEqual(5, ab->ExternalInteger);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(ArrayOfScopeAndAttributeBox)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            {
                std::istringstream array(R"({
                "<AttributeBox>attrArr": [
                    {
                        "string1":"abc",
                        "type":"Scope"
                    },
                    {
                        "float":2.5,
                        "string":"def",
                        "type":"AttributeBox"
                    }
                ]
                })"s);

                IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

                auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
                auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
                fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
                fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));

                jp->Deserialize(array, sw);

                Scope* topScope = sw.GetTopScope();
                Datum* d = topScope->Find(string("attrArr"));

                Scope* arrEle1 = d->GetAsTable(0);
                Scope* arrEle2 = d->GetAsTable(1);

                AttributeBox* ab1 = arrEle1->As<AttributeBox>();
                Assert::IsNull(ab1);

                Assert::AreEqual(string("abc"), arrEle1->Find(string("string1"))->GetAsString(0));

                Datum* d1 = arrEle1->Find(string("type"));
                Assert::IsNull(d1);

                AttributeBox* ab2 = arrEle2->As<AttributeBox>();
                Assert::IsNotNull(ab2);

                Assert::AreEqual(2.5f, ab2->Find(string("float"))->GetAsFloat(0));
                Assert::AreEqual(string("def"), ab2->Find(string("string"))->GetAsString(0));

                Datum* d2 = ab2->Find(string("type"));
                Assert::IsNull(d2);
            }

            {
                std::istringstream complex(R"({
                "ints":1,
                "<AttributeBox>child" : {
                    "childArr" : [
                        {
                            "string1":"abc",
                            "type":"AttributeBox"
                        }, 
                        {
                            "int1":5
                        },
                        {
                            "float1":5.5,
                            "type":"AttributeBox"
                        }
                    ]
                }
            })"s);

                IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

                auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
                auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
                fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
                fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));

                jp->Deserialize(complex, sw);

                Scope* topScope = sw.GetTopScope();
                Datum* d = topScope->Find(string("child"));

                AttributeBox* ab1 = d->GetAsTable(0)->As<AttributeBox>();
                Assert::IsNotNull(ab1);

                Scope* childScope = d->GetAsTable(0);
                Datum* d1 = childScope->Find(string("childArr"));

                Scope* newChild1Scope = d1->GetAsTable(0);
                Scope* newChild2Scope = d1->GetAsTable(1);
                Scope* newChild3Scope = d1->GetAsTable(2);

                AttributeBox* ab2 = newChild1Scope->As<AttributeBox>();
                Assert::IsNotNull(ab2);
                Assert::AreEqual(string("abc"), ab2->Find(string("string1"))->GetAsString(0));

                AttributeBox* ab3 = newChild2Scope->As<AttributeBox>();
                Assert::IsNull(ab3);
                Assert::AreEqual(5, newChild2Scope->Find(string("int1"))->GetAsInt(0));

                AttributeBox* ab4 = newChild3Scope->As<AttributeBox>();
                Assert::IsNotNull(ab4);
                Assert::AreEqual(5.5f, ab4->Find(string("float1"))->GetAsFloat(0));
            }

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(AttributeBoxArrayTest)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            std::istringstream nested(R"({
                "int":1,
                "<AttributeBox>child":{
                    "int":2,
                    "DefIntArrVal": [
                        1,
                        2,
                        3
                    ]
                }
            })"s);

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));

            jp->Deserialize(nested, sw);

            Scope* topScope = sw.GetTopScope();

            Assert::AreEqual(1, topScope->Find(string("int"))->GetAsInt(0));

            Datum* childDatum = topScope->Find(string("child"));
            Scope* childScope = childDatum->GetAsTable(0);

            AttributeBox* ab = childScope->As<AttributeBox>();

            Assert::AreEqual(2, ab->Find("int")->GetAsInt(0));

            Assert::AreEqual(1, ab->ExternalIntegerArray[0]);
            Assert::AreEqual(2, ab->ExternalIntegerArray[1]);
            Assert::AreEqual(3, ab->ExternalIntegerArray[2]);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(VecAndMat)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<SpecialPrimitivesHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            std::istringstream simple(R"json({
                "vec1":"vec4(2.5,3.5,4.5,5.5)",
                "mat1":"mat4x4((1.0,0.0,0.0,0.0), (0.0,1.0,0.0,0.0), (0.0,0.0,1.0,0.0), (0.0,0.0,0.0,1.0))"
                }
            })json"s);

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
            fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));

            jp->Deserialize(simple, sw);

            Scope* topScope = sw.GetTopScope();
            glm::vec4 v1({ 2.5,3.5,4.5,5.5 });
            Assert::AreEqual(v1, topScope->Find("vec1")->GetAsVec4(0));

            glm::mat4x4 mat1({ 1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,1.0 });
            Assert::AreEqual(mat1, topScope->Find("mat1")->GetAsMat4x4(0));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(GameObjectJson)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<GameObjectHandler>());
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<SpecialPrimitivesHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            std::istringstream array(R"Json({
                "<GameObject>attrArr": [
                    {
                        "string1":"abc",
                        "type":"AttributeBox"
                    },
                    {
                        "Position":"vec4(2.5,3.5,4.5,5.5)",
                        "string":"def",
                        "type":"GameObject"
                    }
                ]
                })Json"s);

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
            auto gameObjFactory = std::make_unique<Fiea::Engine::GameObjectFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
            fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));
            fs->Register<Fiea::Engine::GameObject>(std::move(gameObjFactory));

            jp->Deserialize(array, sw);

            Scope* topScope = sw.GetTopScope();
            Datum* d = topScope->Find(string("attrArr"));

            Scope* child1 = d->GetAsTable(0);
            Assert::IsNotNull(child1->As<AttributeBox>());

            Scope* child2 = d->GetAsTable(1);
            Assert::IsNotNull(child2->As<GameObject>());
            glm::vec4 posVec{ 2.5,3.5,4.5,5.5 };
            Assert::AreEqual(posVec, child2->As<GameObject>()->GetTransform().Position);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(GameObjectChildren)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<GameObjectHandler>());
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<SpecialPrimitivesHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            std::istringstream array(R"Json({
                "<GameObject>parentGO": 
                {
                    "<GameObject>childGO":
                    {
                        "Position":"vec4(2.5,3.5,4.5,5.5)"
                    },
                    "Position":"vec4(2.5,3.5,4.5,5.5)"
                }
                })Json"s);

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
            auto gameObjFactory = std::make_unique<Fiea::Engine::GameObjectFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
            fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));
            fs->Register<Fiea::Engine::GameObject>(std::move(gameObjFactory));

            jp->Deserialize(array, sw);

            Scope* topScope = sw.GetTopScope();
            Datum* d = topScope->Find("parentGO");
            GameObject* parentGO = d->GetAsTable(0)->As<GameObject>();
            GameObject* childGO = parentGO->Find("childGO")->GetAsTable(0)->As<GameObject>();

            Assert::AreEqual(1_z, parentGO->GetChildCount());
            Assert::AreEqual(childGO, parentGO->GetChild(0));

            glm::vec4 posVec{ 2.5,3.5,4.5,5.5 };
            Assert::AreEqual(posVec, childGO->GetTransform().Position);
            Assert::AreEqual(posVec, parentGO->GetTransform().Position);

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(RegularAction)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<GameObjectHandler>());
            jc->AddHandler(std::make_unique<ActionHandler>());
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<SpecialPrimitivesHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
            auto gameObjFactory = std::make_unique<Fiea::Engine::GameObjectFactory>();
            auto actionFactory = std::make_unique<Fiea::Engine::Actions::ActionFactory>();
            auto actionIncFactory = std::make_unique<Fiea::Engine::Actions::ActionIncrementFactory>();
            auto actionAddDatumFactory = std::make_unique<Fiea::Engine::Actions::ActionAddDatumFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
            fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));
            fs->Register<Fiea::Engine::GameObject>(std::move(gameObjFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionIncFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionAddDatumFactory));

            std::istringstream array(R"Json({
                "<GameObject>parentGO": 
                {
                    "<GameObject>childGO":
                    {
                        "Position":"vec4(2.5,3.5,4.5,5.5)",
                        "<Action>regAction":
                        {
                            "ActionName":"regAction"
                        }
                    },
                    "Position":"vec4(2.5,3.5,4.5,5.5)"
                }
                })Json"s);

            jp->Deserialize(array, sw);

            Scope* topScope = sw.GetTopScope();
            Datum* d = topScope->Find("parentGO");
            GameObject* parentGO = d->GetAsTable(0)->As<GameObject>();
            GameObject* childGO = parentGO->Find("childGO")->GetAsTable(0)->As<GameObject>();

            Assert::AreEqual(1_z, parentGO->GetChildCount());
            Assert::AreEqual(childGO, parentGO->GetChild(0));

            glm::vec4 posVec{ 2.5,3.5,4.5,5.5 };
            Assert::AreEqual(posVec, childGO->GetTransform().Position);
            Assert::AreEqual(posVec, parentGO->GetTransform().Position);

            Datum* actionDatum = childGO->Find("regAction");
            Action* a = actionDatum->GetAsTable(0)->As<Action>();
            Assert::IsNotNull(a);
            Assert::AreEqual("regAction", a->_ActionName.c_str());

            Assert::AreEqual(1_z, childGO->Find("Actions")->Size());

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(AdditionalActions)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<GameObjectHandler>());
            jc->AddHandler(std::make_unique<ActionHandler>());
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<SpecialPrimitivesHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
            auto gameObjFactory = std::make_unique<Fiea::Engine::GameObjectFactory>();
            auto actionFactory = std::make_unique<Fiea::Engine::Actions::ActionFactory>();
            auto actionIncFactory = std::make_unique<Fiea::Engine::Actions::ActionIncrementFactory>();
            auto actionAddDatumFactory = std::make_unique<Fiea::Engine::Actions::ActionAddDatumFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
            fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));
            fs->Register<Fiea::Engine::GameObject>(std::move(gameObjFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionIncFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionAddDatumFactory));

            std::istringstream actionIncJson(R"Json({
                "<GameObject>parentGO": 
                {
                    "<GameObject>childGO":
                    {
                        "Position":"vec4(2.5,3.5,4.5,5.5)",
                        "testKey":2.4,
                        "<ActionIncrement>incAction":
                        {
                            "ActionName":"incAction",
                            "DatumKey":"testKey",
                            "IncrementAmount":8.5
                        },
                        "<ActionAddDatum>addDatumAction":
                        {
                            "ActionName":"addDatumAction",
                            "DatumKey":"datumKey",
                            "DatumStr":"datumStr",
                            "RunTime":2.0
                        }
                    },
                    "Position":"vec4(2.5,3.5,4.5,5.5)"
                }
                })Json"s);

            jp->Deserialize(actionIncJson, sw);

            Scope* topScope = sw.GetTopScope();
            Datum* d = topScope->Find("parentGO");
            GameObject* parentGO = d->GetAsTable(0)->As<GameObject>();
            GameObject* childGO = parentGO->Find("childGO")->GetAsTable(0)->As<GameObject>();

            Assert::AreEqual(1_z, parentGO->GetChildCount());
            Assert::AreEqual(childGO, parentGO->GetChild(0));

            glm::vec4 posVec{ 2.5,3.5,4.5,5.5 };
            Assert::AreEqual(posVec, childGO->GetTransform().Position);
            Assert::AreEqual(posVec, parentGO->GetTransform().Position);

            Datum* actionDatum = childGO->Find("incAction");
            ActionIncrement* a = actionDatum->GetAsTable(0)->As<ActionIncrement>();
            Assert::IsNotNull(a);
            Assert::AreEqual("incAction", a->_ActionName.c_str());
            Assert::AreEqual(8.5f, a->_IncAmount);
            while (a->Update() != true)
            {
                
            }
            Assert::AreEqual(10.9f, childGO->Find("testKey")->GetAsFloat(0));

            Datum* addDatumAction = childGO->Find("addDatumAction");
            ActionAddDatum* aa = addDatumAction->GetAsTable(0)->As<ActionAddDatum>();
            Assert::IsNotNull(aa);
            Assert::AreEqual("addDatumAction", aa->_ActionName.c_str());
            while (aa->Update() != true)
            {
                Datum* addedDatum = childGO->Find("datumKey");
                if (addedDatum != nullptr)
                {
                    Assert::AreEqual("datumStr", addedDatum->GetAsString(0).c_str());
                }
            }
            Assert::IsNull(childGO->Find("datumKey"));

            Assert::AreEqual(2_z, childGO->Find("Actions")->Size());

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(ActionList)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<GameObjectHandler>());
            jc->AddHandler(std::make_unique<ActionListHandler>());
            jc->AddHandler(std::make_unique<ActionHandler>());
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<SpecialPrimitivesHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
            auto gameObjFactory = std::make_unique<Fiea::Engine::GameObjectFactory>();
            auto actionFactory = std::make_unique<Fiea::Engine::Actions::ActionFactory>();
            auto actionIncFactory = std::make_unique<Fiea::Engine::Actions::ActionIncrementFactory>();
            auto actionAddDatumFactory = std::make_unique<Fiea::Engine::Actions::ActionAddDatumFactory>();
            auto actionListFactory = std::make_unique<Fiea::Engine::Actions::ActionListFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
            fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));
            fs->Register<Fiea::Engine::GameObject>(std::move(gameObjFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionIncFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionAddDatumFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionListFactory));

            std::istringstream actionListJson(R"Json({
                "<GameObject>parentGO": 
                {
                    "<GameObject>childGO":
                    {
                        "testKey":2.4,
                        "<ActionList>list":
                        {
                            "<ActionIncrement>incAction":
                            {
                                "ActionName":"incAction",
                                "DatumKey":"testKey",
                                "IncrementAmount":8.5
                            },
                            "<ActionAddDatum>addDatumAction":
                            {
                                "ActionName":"addDatumAction",
                                "DatumKey":"datumKey",
                                "DatumStr":"datumStr",
                                "RunTime":2.0
                            }
                        }
                    }
                }
                })Json"s);

            jp->Deserialize(actionListJson, sw);

            Scope* topScope = sw.GetTopScope();
            Datum* d = topScope->Find("parentGO");
            GameObject* parentGO = d->GetAsTable(0)->As<GameObject>();
            GameObject* childGO = parentGO->Find("childGO")->GetAsTable(0)->As<GameObject>();

            Scope* actionListScope = childGO->Find("list")->GetAsTable(0);
            Fiea::Engine::Actions::ActionList* al = actionListScope->As< Fiea::Engine::Actions::ActionList>();
            Assert::IsNotNull(al);

            while (al->Update() != true)
            {
                Datum* addedDatum = actionListScope->Find("datumKey");
                if (addedDatum != nullptr)
                {
                    Assert::AreEqual("datumStr", addedDatum->GetAsString(0).c_str());
                }
            }
            Assert::AreEqual(10.9f, childGO->Find("testKey")->GetAsFloat(0));
            Assert::IsNull(actionListScope->Find("datumKey"));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(WhileLoopAction)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<GameObjectHandler>());
            jc->AddHandler(std::make_unique<ActionListHandler>());
            jc->AddHandler(std::make_unique<ActionWhileParamHandler>());
            jc->AddHandler(std::make_unique<ActionHandler>());
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<SpecialPrimitivesHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
            auto gameObjFactory = std::make_unique<Fiea::Engine::GameObjectFactory>();
            auto actionFactory = std::make_unique<Fiea::Engine::Actions::ActionFactory>();
            auto actionIncFactory = std::make_unique<Fiea::Engine::Actions::ActionIncrementFactory>();
            auto actionAddDatumFactory = std::make_unique<Fiea::Engine::Actions::ActionAddDatumFactory>();
            auto actionPreambleFactory = std::make_unique<Fiea::Engine::Actions::ActionPreambleFactory>();
            auto actionConditionFactory = std::make_unique<Fiea::Engine::Actions::ActionConditionFactory>();
            auto actionLoopIncrementFactory = std::make_unique<Fiea::Engine::Actions::ActionLoopIncrementFactory>();
            auto actionListFactory = std::make_unique<Fiea::Engine::Actions::ActionListFactory>();
            auto actionWhileLoopFactory = std::make_unique<Fiea::Engine::Actions::ActionListWhileLoopFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
            fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));
            fs->Register<Fiea::Engine::GameObject>(std::move(gameObjFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionIncFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionAddDatumFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionPreambleFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionConditionFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionLoopIncrementFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionListFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionWhileLoopFactory));

            std::istringstream actionListJson(R"Json({
                "<GameObject>parentGO": 
                {
                    "<GameObject>childGO":
                    {
                        "testKey":2.5,
                        "<ActionListWhileLoop>whileAction":
                        {
                            "Counter":0,
                            "Limit":5,
                            "<ActionPreamble>Preamble":
                            {
                                
                            },
                            "<ActionCondition>Condition":
                            {
                                
                            },
                            "<ActionLoopIncrement>Increment":
                            {
                                "IncrementAmount":1
                            },
                            "<ActionIncrement>incAction":
                            {
                                "ActionName":"incAction",
                                "DatumKey":"testKey",
                                "IncrementAmount":0.5
                            }
                        }
                    }
                }
                })Json"s);

            jp->Deserialize(actionListJson, sw);

            Scope* topScope = sw.GetTopScope();
            Datum* d = topScope->Find("parentGO");
            GameObject* parentGO = d->GetAsTable(0)->As<GameObject>();
            GameObject* childGO = parentGO->Find("childGO")->GetAsTable(0)->As<GameObject>();

            Scope* actionListScope = childGO->Find("whileAction")->GetAsTable(0);
            Fiea::Engine::Actions::ActionListWhileLoop* awl = actionListScope->As<Fiea::Engine::Actions::ActionListWhileLoop>();
            Assert::IsNotNull(awl);

            while (awl->Update() != true)
            {
                
            }
            Assert::AreEqual(5.0f, childGO->Find("testKey")->GetAsFloat(0));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(TimedAction)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<GameObjectHandler>());
            jc->AddHandler(std::make_unique<ActionListHandler>());
            jc->AddHandler(std::make_unique<ActionWhileParamHandler>());
            jc->AddHandler(std::make_unique<ActionHandler>());
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<SpecialPrimitivesHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
            auto gameObjFactory = std::make_unique<Fiea::Engine::GameObjectFactory>();
            auto actionFactory = std::make_unique<Fiea::Engine::Actions::ActionFactory>();
            auto actionIncFactory = std::make_unique<Fiea::Engine::Actions::ActionIncrementFactory>();
            auto actionAddDatumFactory = std::make_unique<Fiea::Engine::Actions::ActionAddDatumFactory>();
            auto actionPreambleFactory = std::make_unique<Fiea::Engine::Actions::ActionPreambleFactory>();
            auto actionConditionFactory = std::make_unique<Fiea::Engine::Actions::ActionConditionFactory>();
            auto actionLoopIncrementFactory = std::make_unique<Fiea::Engine::Actions::ActionLoopIncrementFactory>();
            auto actionTimedFactory = std::make_unique<Fiea::Engine::Actions::ActionTimedFactory>();
            auto actionDelayedFactory = std::make_unique<Fiea::Engine::Actions::ActionDelayedFactory>();
            auto actionListFactory = std::make_unique<Fiea::Engine::Actions::ActionListFactory>();
            auto actionWhileLoopFactory = std::make_unique<Fiea::Engine::Actions::ActionListWhileLoopFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
            fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));
            fs->Register<Fiea::Engine::GameObject>(std::move(gameObjFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionIncFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionAddDatumFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionPreambleFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionConditionFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionLoopIncrementFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionTimedFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionDelayedFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionListFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionWhileLoopFactory));

            std::istringstream actionListJson(R"Json({
                "<GameObject>parentGO": 
                {
                    "<GameObject>childGO":
                    {
                        "testKey":2.5,
                        "testKey2":3.0,
                        "<ActionTimed>timedAction":
                        {
                            "RunTime":3.0,
                            "<ActionIncrement>incAction":
                            {
                                "ActionName":"incAction",
                                "DatumKey":"testKey",
                                "IncrementAmount":0.5
                            }
                        },
                        "<ActionDelayed>delayedAction":
                        {
                            "Delay":2.0,
                            "RepeatCount":2,
                            "<ActionIncrement>incAction":
                            {
                                "ActionName":"incAction",
                                "DatumKey":"testKey2",
                                "IncrementAmount":0.5
                            }
                        }
                    }
                }
                })Json"s);

            jp->Deserialize(actionListJson, sw);

            Scope* topScope = sw.GetTopScope();
            Datum* d = topScope->Find("parentGO");
            GameObject* parentGO = d->GetAsTable(0)->As<GameObject>();
            GameObject* childGO = parentGO->Find("childGO")->GetAsTable(0)->As<GameObject>();

            Scope* delayedActionScope = childGO->Find("delayedAction")->GetAsTable(0);
            Fiea::Engine::Actions::ActionDelayed* ad = delayedActionScope->As<Fiea::Engine::Actions::ActionDelayed>();
            Assert::IsNotNull(ad);
            while (ad->Update() != true)
            {

            }
            Assert::AreEqual(4.0f, childGO->Find("testKey2")->GetAsFloat(0));
            
            Scope* timedActionScope = childGO->Find("timedAction")->GetAsTable(0);
            Fiea::Engine::Actions::ActionTimed* at = timedActionScope->As<Fiea::Engine::Actions::ActionTimed>();
            Assert::IsNotNull(at);
            Datum* abcd = childGO->Find("testKey");
            while (at->Update() != true)
            {
                
            }
            Datum* timedActionDatum = childGO->Find("testKey");
            Assert::AreEqual(3.0f, childGO->Find("testKey")->GetAsFloat(0));

            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(HealAndDamageTest)
        {
            IJsonConfig* jc = ServiceManager::ProvideInterface<IJsonConfig>();
            jc->AddHandler(std::make_unique<GameObjectHandler>());
            jc->AddHandler(std::make_unique<ActionListHandler>());
            jc->AddHandler(std::make_unique<ActionWhileParamHandler>());
            jc->AddHandler(std::make_unique<ActionHandler>());
            jc->AddHandler(std::make_unique<ScopeFactoryHandler>());
            jc->AddHandler(std::make_unique<SpecialPrimitivesHandler>());
            jc->AddHandler(std::make_unique<ScopeHandler>());
            jc->AddHandler(std::make_unique<PrimitiveTypeHandler>());

            IJsonParser* jp = ServiceManager::ProvideInterface<IJsonParser>();

            ScopeWriter sw;

            IFactoryService* fs = ServiceManager::ProvideInterface<IFactoryService>();

            auto attrBoxFactory = std::make_unique<Fiea::Engine::Tests::Support::AttributeBoxFactory>();
            auto scopeFactory = std::make_unique<Fiea::Engine::Content::ScopeFactory>();
            auto gameObjFactory = std::make_unique<Fiea::Engine::GameObjectFactory>();
            auto actionFactory = std::make_unique<Fiea::Engine::Actions::ActionFactory>();
            auto actionIncFactory = std::make_unique<Fiea::Engine::Actions::ActionIncrementFactory>();
            auto actionDecFactory = std::make_unique<Fiea::Engine::Actions::ActionDecrementFactory>();
            auto actionAddDatumFactory = std::make_unique<Fiea::Engine::Actions::ActionAddDatumFactory>();
            auto actionPreambleFactory = std::make_unique<Fiea::Engine::Actions::ActionPreambleFactory>();
            auto actionConditionFactory = std::make_unique<Fiea::Engine::Actions::ActionConditionFactory>();
            auto actionLoopIncrementFactory = std::make_unique<Fiea::Engine::Actions::ActionLoopIncrementFactory>();
            auto actionTimedFactory = std::make_unique<Fiea::Engine::Actions::ActionTimedFactory>();
            auto actionDelayedFactory = std::make_unique<Fiea::Engine::Actions::ActionDelayedFactory>();
            auto actionOutputFactory = std::make_unique<Fiea::Engine::Actions::ActionOutputFactory>();
            auto actionConditionalIncFactory = std::make_unique<Fiea::Engine::Actions::ActionConditionalIncrementFactory>();
            auto actionListFactory = std::make_unique<Fiea::Engine::Actions::ActionListFactory>();
            auto actionWhileLoopFactory = std::make_unique<Fiea::Engine::Actions::ActionListWhileLoopFactory>();
            fs->Register<Fiea::Engine::Content::Scope>(std::move(attrBoxFactory));
            fs->Register<Fiea::Engine::Content::Scope>(std::move(scopeFactory));
            fs->Register<Fiea::Engine::GameObject>(std::move(gameObjFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionIncFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionDecFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionAddDatumFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionPreambleFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionConditionFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionLoopIncrementFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionTimedFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionDelayedFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionOutputFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionConditionalIncFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionListFactory));
            fs->Register<Fiea::Engine::Actions::Action>(std::move(actionWhileLoopFactory));

            std::istringstream actionListJson(R"Json({
                "<GameObject>Player": 
                {
                    "Health":100.0,
                    "MaxHealth":100.0,
                    "Mana":50.0,
                    "<ActionOutput>OutputAction":
                    {
                        "OutputDatum":"Health",
                        "Delay":1.0,
                        "RepeatCount":0
                    },
                    "<ActionDelayed>DamageAction":
                    {
                        "Delay":5.0,
                        "RepeatCount":0,
                        "<ActionDecrement>ReduceHealth":
                        {
                            "ActionName":"ReduceHealth",
                            "DatumKey":"Health",
                            "DecrementAmount":40.0,
                            "MinVal":0.0
                        }
                    },
                    "<ActionDelayed>HealAction":
                    {
                        "Delay":8.0,
                        "RepeatCount":0,
                        "<ActionDelayed>ActualHealing":
                        { 
                            "Delay":0.5,
                            "RepeatCount":3,
                            "Counter":50,
                            "Limit":0,
                            "<ActionConditionalIncrement>HealSelf":
                            {                                
                                "<ActionCondition>Condition":
                                {
                                    
                                },
                                "ActionName":"HealSelf",
                                "DatumKey":"Health",
                                "IncrementAmount":2.0,
                                "MaxVal":100.0,
                                "<ActionDecrement>ReduceMana":
                                {
                                    "ActionName":"ReduceMana",
                                    "DatumKey":"Mana",
                                    "DecrementAmount":10.0,
                                    "MinVal":0.0
                                }
                            }
                        }
                    }
                }
                })Json"s);

            jp->Deserialize(actionListJson, sw);

            Scope* topScope = sw.GetTopScope();
            Datum* d = topScope->Find("Player");
            GameObject* parentGO = d->GetAsTable(0)->As<GameObject>();

            int32_t i = 5000000;
            while (i >= 0)
            {
                parentGO->Update(1.0);
                i--;
            }

            ServiceManager::ResetServiceMgr();
        }
    };
}