#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "../src/Actions/ActionIncrement.h"
#include "../src/Actions/ActionAddDatum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Content;
using namespace Fiea::Engine::Actions;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(ActionIncrementTest)
    {
        TEST_MEMCHECK;

        TEST_METHOD(SimpleAction)
        {
            Scope s;
            Datum& d = s.Append("IncDatum");
            d.SetType(Datum::_DatumType::Float);
            d.Push(2.5f);
            ActionIncrement* a = new ActionIncrement();
            
            s.AppendScope("ActionInc", a);

            while (a->Update() != true)
            {

            }
            Assert::AreEqual(2.5f, d.GetAsFloat(0));

            ServiceManager::ResetServiceMgr();
        }
    };

    TEST_CLASS(ActionAddDatumTest)
    {
        TEST_MEMCHECK;

        TEST_METHOD(UpdateTest)
        {
            Scope s;
            ActionAddDatum* a = new ActionAddDatum();

            s.AppendScope("ActionAddDatum", a);

            while (a->Update() != true)
            {
                Datum* d = s.Find("TestKey");
                if (d != nullptr)
                {
                    if (d->Size() > 0)
                    {
                        Assert::AreEqual("TestStr", d->GetAsString(0).c_str());
                        Assert::AreEqual(1_z, d->Size());
                    }
                }
            }
            Datum* d = s.Find("TestKey");
            Assert::IsNull(d);

            ServiceManager::ResetServiceMgr();
        }
    };
}