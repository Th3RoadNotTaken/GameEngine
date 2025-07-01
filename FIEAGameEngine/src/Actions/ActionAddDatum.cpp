#include "ActionAddDatum.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionAddDatum);

    ClassDefinition ActionAddDatum::CreateDefinitions()
    {
        return {
        { "DatumKey", offsetof(ActionAddDatum, _DatumKey), Datum::_DatumType::String, 1},
        { "DatumStr", offsetof(ActionAddDatum, _DatumStr), Datum::_DatumType::String, 1},
        { "RunTime", offsetof(ActionAddDatum, _RunTime), Datum::_DatumType::Float, 1}
        };
    }

    ActionAddDatum::ActionAddDatum() : Action(CreateDefinitions(), TypeIdInstance()), _RunTime(3.0f)
    {
        _DatumKey = "TestKey";
        _DatumStr = "TestStr";
    }

    bool ActionAddDatum::InitAction()
    {
        Scope* parent = GetParent();
        FIEA_ERROR(parent != nullptr);

        Datum* d = parent->Find(_DatumKey.c_str());
        if (d != nullptr) return true;

        Datum& appendedDatum = parent->Append(_DatumKey.c_str());
        appendedDatum.Push<string>(_DatumStr.c_str());

        _StartTime = std::chrono::steady_clock::now();

        return false;
    }

    bool ActionAddDatum::RunAction()
    {
        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - _StartTime).count();
    
        if (elapsed >= _RunTime)
        {
            Scope* parent = GetParent();
            FIEA_ERROR(parent != nullptr);

            /*Datum* d = parent->Find(_DatumKey.c_str());

            if (d != nullptr)
            {
                d->Pop();
            }*/
            parent->RemoveDatum(_DatumKey.c_str());
            return true;
        }

        return false;
    }

    void ActionAddDatum::CleanupAction()
    {
    }

    FACTORY_DEFINE(ActionAddDatum);
}
FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);