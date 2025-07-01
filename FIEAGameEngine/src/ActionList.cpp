#include "FIEAGameEngine/ActionList.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionList);

    ClassDefinition ActionList::CreateDefinitions()
    {
        return {
            { "ActionList", 0, Datum::_DatumType::Table, 1 }
        };
    }

    ActionList::ActionList() : Action(CreateDefinitions(), TypeIdInstance())
    {
    }

    ActionList::ActionList(const ClassDefinition& other, const RTTI::IdType& idType) : Action(other, idType)
    {
    }

    bool ActionList::InitAction()
    {
        return false;
    }

    bool ActionList::RunAction()
    {
        Datum* actionDatum = Find("ActionList");
        if (actionDatum == nullptr)
            return true;

        if (actionDatum->Size() == 0) return true;

        for (size_t i = 0; i < actionDatum->Size() ; ++i)
        {
            Action* action = actionDatum->GetAsTable(i)->As<Action>();
            FIEA_ERROR(action != nullptr);
            
            bool completed = action->Update();
            if (!completed)
            {
                return false;
            }
        }
        return true;
    }

    void ActionList::CleanupAction()
    {
        Datum* actionDatum = Find("ActionList");
        if (actionDatum == nullptr)
            return;

        if (actionDatum->Size() == 0) return;

        for (size_t i = 0; i < actionDatum->Size(); ++i)
        {
            Action* action = actionDatum->GetAsTable(i)->As<Action>();
            FIEA_ERROR(action != nullptr);

            action->CleanupAction();
            delete action;
        }

        actionDatum->Clear();
    }

    void ActionList::AddAction(Action* action)
    {
        if (action == nullptr)
            return;

        action->SetParent(*this);

        Datum* actionsDatum = Find("ActionList");
        if (actionsDatum == nullptr)
        {
            actionsDatum = &Append("ActionList");
            actionsDatum->SetType(Datum::_DatumType::Table);
        }
        actionsDatum->Push(action);
    }

    bool ActionList::RemoveAction(Action* action)
    {
        if (action == nullptr)
            return false;

        Datum* actionsDatum = Find("ActionList");
        if (actionsDatum != nullptr)
        {
            for (size_t i = 0; i < actionsDatum->Size(); ++i)
            {
                if (actionsDatum->GetAsTable(i)->As<Action>() == action)
                {
                    actionsDatum->RemoveAt(i);
                    return true;
                }
            }
        }
        return false;
    }

    FACTORY_DEFINE(ActionList);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);