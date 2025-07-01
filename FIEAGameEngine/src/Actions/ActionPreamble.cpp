#include "ActionPreamble.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionPreamble);

    ClassDefinition ActionPreamble::CreateDefinitions()
    {
        return {};
    }

    ActionPreamble::ActionPreamble() : Action(CreateDefinitions(), TypeIdInstance())
    {
    }

    bool ActionPreamble::InitAction()
    {
        Datum* d = Search("Counter");
        FIEA_ERROR(d != nullptr);

        d->Set(0, 0);
        return false;
    }

    bool ActionPreamble::RunAction()
    {
        Datum* d = Search("Counter");
        FIEA_ERROR(d != nullptr);

        d->Set(0, 0);
        return true;
    }

    void ActionPreamble::CleanupAction()
    {
    }

    FACTORY_DEFINE(ActionPreamble);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);