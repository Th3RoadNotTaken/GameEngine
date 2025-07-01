#include "ActionConditionalIncrement.h"
#include "ActionCondition.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionConditionalIncrement);

    ClassDefinition ActionConditionalIncrement::CreateDefinitions()
    {
        return {
        { "DatumKey", offsetof(ActionConditionalIncrement, _DatumKey), Datum::_DatumType::String, 1},
        { "IncrementAmount", offsetof(ActionConditionalIncrement, _IncAmount), Datum::_DatumType::Float, 1},
        { "MaxVal", offsetof(ActionConditionalIncrement, _MaxVal), Datum::_DatumType::Float, 1},
        { "Condition", 0 , Datum::_DatumType::Table, 1 }
        };
    }

    ActionConditionalIncrement::ActionConditionalIncrement() : Action(CreateDefinitions(), TypeIdInstance())
    {
        _DatumKey = "IncDatum";
        _IncAmount = 0.0f;
        _MaxVal = -999999.0f;
    }

    bool ActionConditionalIncrement::InitAction()
    {
        return false;
    }

    bool ActionConditionalIncrement::RunAction()
    {
        Scope* conditonScope = Find("Condition")->GetAsTable(0);
        FIEA_ERROR(conditonScope != nullptr);

        ActionCondition* ac = conditonScope->As<ActionCondition>();
        FIEA_ERROR(ac != nullptr);

        if (ac->Update() == true)
            return true;

        Datum* d = Search(_DatumKey.c_str());
        FIEA_ASSERT(d != nullptr && d->IsType(Datum::_DatumType::Float));

        float currentVal = d->GetAsFloat(0);
        float newVal = currentVal + _IncAmount;
        if (_MaxVal != -999999.0f)
        {
            if (newVal > _MaxVal)
                newVal = _MaxVal;
        }

        d->Set(static_cast<float>(newVal), 0);

        return false;
    }

    void ActionConditionalIncrement::CleanupAction()
    {
    }

    FACTORY_DEFINE(ActionConditionalIncrement);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);