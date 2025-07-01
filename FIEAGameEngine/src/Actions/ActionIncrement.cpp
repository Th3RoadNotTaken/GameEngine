#include "ActionIncrement.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionIncrement);

    ClassDefinition ActionIncrement::CreateDefinitions()
    {
        return {
        { "DatumKey", offsetof(ActionIncrement, _DatumKey), Datum::_DatumType::String, 1},
        { "IncrementAmount", offsetof(ActionIncrement, _IncAmount), Datum::_DatumType::Float, 1},
        { "MaxVal", offsetof(ActionIncrement, _MaxVal), Datum::_DatumType::Float, 1}
        };
    }

    ActionIncrement::ActionIncrement() : Action(CreateDefinitions(), TypeIdInstance())
    {
        _DatumKey = "IncDatum";
        _IncAmount = 0.0f;
        _MaxVal = -999999.0f;
    }

    bool ActionIncrement::InitAction()
    {
        return false;
    }

    bool ActionIncrement::RunAction()
    {
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
        
        return true;
    }

    void ActionIncrement::CleanupAction()
    {
    }

    FACTORY_DEFINE(ActionIncrement);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);