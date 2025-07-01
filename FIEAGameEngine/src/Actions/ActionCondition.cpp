#include "ActionCondition.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionCondition);

    ClassDefinition ActionCondition::CreateDefinitions()
    {
        return {};
    }

    ActionCondition::ActionCondition() : Action(CreateDefinitions(), TypeIdInstance()), _IncCondition(true)
    {
    }

    bool ActionCondition::InitAction()
    {
        Datum* counterDatum = Search("Counter");
        Datum* limitDatum = Search("Limit");
        FIEA_ERROR(counterDatum != nullptr && limitDatum != nullptr);

        if (counterDatum->IsType(Datum::_DatumType::Int) && limitDatum->IsType(Datum::_DatumType::Int))
        {
            if (counterDatum->GetAsInt(0) <= limitDatum->GetAsInt(0))
                _IncCondition = true;
            else
                _IncCondition = false;
        }
        else if (counterDatum->IsType(Datum::_DatumType::Float) && limitDatum->IsType(Datum::_DatumType::Float))
        {
            if (counterDatum->GetAsFloat(0) <= limitDatum->GetAsFloat(0))
                _IncCondition = true;
            else
                _IncCondition = false;
        }

        return false;
    }

    bool ActionCondition::RunAction()
    {
        Datum* counterDatum = Search("Counter");
        Datum* limitDatum = Search("Limit");
        FIEA_ERROR(counterDatum != nullptr && limitDatum != nullptr);

        if (counterDatum->IsType(Datum::_DatumType::Int) && limitDatum->IsType(Datum::_DatumType::Int))
        {
            if (_IncCondition)
            {
                // Increment condition check
                if (counterDatum->GetAsInt(0) < limitDatum->GetAsInt(0))
                    return false;
                else
                    return true;
            }
            else
            {
                // Decrement condition check
                if (counterDatum->GetAsInt(0) > limitDatum->GetAsInt(0))
                    return false;
                else
                    return true;
            }
        }
        else if (counterDatum->IsType(Datum::_DatumType::Float) && limitDatum->IsType(Datum::_DatumType::Float))
        {
            if (_IncCondition)
            {
                // Increment condition check
                if (counterDatum->GetAsFloat(0) < limitDatum->GetAsFloat(0))
                    return false;
                else
                    return true;
            }
            else
            {
                // Decrement condition check
                if (counterDatum->GetAsFloat(0) > limitDatum->GetAsFloat(0))
                    return false;
                else
                    return true;

            }
        }
        FIEA_ERROR(counterDatum->IsType(Datum::_DatumType::Float) ||
            counterDatum->IsType(Datum::_DatumType::Int) ||
            limitDatum->IsType(Datum::_DatumType::Float) ||
            limitDatum->IsType(Datum::_DatumType::Int));

        return true;
    }

    void ActionCondition::CleanupAction()
    {
    }

    FACTORY_DEFINE(ActionCondition);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);