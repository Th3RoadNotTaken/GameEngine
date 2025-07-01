#include "ActionListWhileLoop.h"
#include "ActionPreamble.h"
#include "ActionCondition.h"
#include "ActionLoopIncrement.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionListWhileLoop);

    ClassDefinition ActionListWhileLoop::CreateDefinitions()
    {
        return {
            { "Counter", offsetof(ActionListWhileLoop, _Counter), Datum::_DatumType::Int, 1 },
            { "Limit", offsetof(ActionListWhileLoop, _Limit), Datum::_DatumType::Int, 1 },
            { "Preamble", 0, Datum::_DatumType::Table, 1 },
            { "Condition", 0 , Datum::_DatumType::Table, 1 },
            { "Increment", 0 , Datum::_DatumType::Table, 1 }
        };
    }

    ActionListWhileLoop::ActionListWhileLoop() : ActionList(CreateDefinitions(), TypeIdInstance()), _Counter(0), _Limit(0)
    {
    }

    bool ActionListWhileLoop::InitAction()
    {
        Scope* preambleScope = Find("Preamble")->GetAsTable(0);
        FIEA_ERROR(preambleScope != nullptr);

        ActionPreamble* ap = preambleScope->As<ActionPreamble>();
        FIEA_ERROR(ap != nullptr);

        while (ap->Update() != true)
        {

        }

        return false;
    }

    bool ActionListWhileLoop::RunAction()
    {
        Scope* conditionScope = Find("Condition")->GetAsTable(0);
        Scope* incrementScope = Find("Increment")->GetAsTable(0);
        FIEA_ERROR(conditionScope != nullptr && incrementScope != nullptr);

        ActionCondition* ac = conditionScope->As<ActionCondition>();
        ActionLoopIncrement* ali = incrementScope->As<ActionLoopIncrement>();
        FIEA_ERROR(ac != nullptr && ali != nullptr);

        if (ac->Update() == true)
            return true;

        Datum* bodyDatum = Find("ActionList");

        if (bodyDatum != nullptr)
        {
            for (size_t i = 0; i < bodyDatum->Size(); ++i)
            {
                Action* nestedAction = bodyDatum->GetAsTable(i)->As<Action>();
                FIEA_ERROR(nestedAction != nullptr);

                while (nestedAction->Update() != true)
                {
                }
            }
        }

        while (ali->Update() != true)
        {

        }

        return false;
    }

    void ActionListWhileLoop::CleanupAction()
    {
        Datum* bodyDatum = Find("ActionList");
        if (bodyDatum != nullptr)
        {
            for (size_t i = 0; i < bodyDatum->Size(); ++i)
            {
                Action* nestedAction = bodyDatum->GetAsTable(i)->As<Action>();
                FIEA_ASSERT(nestedAction != nullptr);
                nestedAction->CleanupAction();
                delete nestedAction;
            }
            bodyDatum->Clear();
        }
    }

    FACTORY_DEFINE(ActionListWhileLoop);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);