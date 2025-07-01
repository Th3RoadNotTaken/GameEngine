#include "ActionLoopIncrement.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionLoopIncrement);

    ClassDefinition ActionLoopIncrement::CreateDefinitions()
    {
        return {
        { "IncrementAmount", offsetof(ActionLoopIncrement, _IncAmount), Datum::_DatumType::Int, 1}
        };
    }

    ActionLoopIncrement::ActionLoopIncrement() : Action(CreateDefinitions(), TypeIdInstance())
    {
        _IncAmount = 0;
    }

    bool ActionLoopIncrement::InitAction()
    {
        return false;
    }

    bool ActionLoopIncrement::RunAction()
    {
        Datum* d = Search("Counter");
        FIEA_ASSERT(d != nullptr && d->IsType(Datum::_DatumType::Int));

        int32_t currentVal = d->GetAsInt(0);
        int32_t newVal = currentVal + _IncAmount;

        d->Set(static_cast<int32_t>(newVal), 0);

        return true;
    }

    void ActionLoopIncrement::CleanupAction()
    {
    }

    FACTORY_DEFINE(ActionLoopIncrement);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);