#include "ActionDecrement.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionDecrement);

    ClassDefinition ActionDecrement::CreateDefinitions()
    {
        return {
        { "DatumKey", offsetof(ActionDecrement, _DatumKey), Datum::_DatumType::String, 1},
        { "DecrementAmount", offsetof(ActionDecrement, _DecAmount), Datum::_DatumType::Float, 1},
        { "MinVal", offsetof(ActionDecrement, _MinVal), Datum::_DatumType::Float, 1}
        };
    }

    ActionDecrement::ActionDecrement() : Action(CreateDefinitions(), TypeIdInstance())
    {
        _DatumKey = "IncDatum";
        _DecAmount = 0.0f;
        _MinVal = 999999.0f;
    }

    bool ActionDecrement::InitAction()
    {
        return false;
    }

    bool ActionDecrement::RunAction()
    {
        Datum* d = Search(_DatumKey.c_str());
        FIEA_ASSERT(d != nullptr && d->IsType(Datum::_DatumType::Float));

        float currentVal = d->GetAsFloat(0);
        float newVal = currentVal - _DecAmount;;
        if (_MinVal != 999999.0f)
        {
            if (newVal < _MinVal)
                newVal = _MinVal;
        }
        
        d->Set(static_cast<float>(newVal), 0);

        return true;
    }

    void ActionDecrement::CleanupAction()
    {
    }

    FACTORY_DEFINE(ActionDecrement);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);