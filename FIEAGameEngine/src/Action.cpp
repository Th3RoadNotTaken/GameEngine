#include "FIEAGameEngine/Action.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(Action);

    ClassDefinition Action::CreateDefinitions()
    {
        return {
        { "ActionName", offsetof(Action, _ActionName), Datum::_DatumType::String, 1}
        };
    }

    Action::Action() : Attributed(&CreateDefinitions, TypeIdInstance()), _ActionState(ActionState::Idle)
    {
    }

    Action::Action(const Action& other) : Attributed(other), _ActionState(ActionState::Idle)
    {
    }

    Action::Action(Action&& other) noexcept : Attributed(std::move(other)), _ActionState(ActionState::Idle)
    {
    }

    Action& Action::operator=(const Action& other)
    {
        if (this != &other)
        {
            Attributed::operator=(other);
            _ActionState = other._ActionState;
        }
        return *this;
    }

    Action& Action::operator=(Action&& other) noexcept
    {
        if (this != &other)
        {
            _ActionState = std::move(other._ActionState);
            Attributed::operator=(std::move(other));
        }
        return *this;
    }

    ClassDefinition Action::AppendAttributes(const ClassDefinition& def, const ClassDefinition& additionalDef)
    {
        ClassDefinition baseCpy = def;

        for (auto& signature : additionalDef)
        {
            baseCpy.push_back(signature);
        }
        return baseCpy;
    }

    Action::Action(const ClassDefinition& other, const RTTI::IdType& idType) : Attributed([other]() -> ClassDefinition {return AppendAttributes(CreateDefinitions(), other); }, idType)
    {
        _ActionState = ActionState::Idle;
    }

    bool Action::Update()
    {
        switch (_ActionState)
        {
        case ActionState::Idle:
            if (InitAction())
            {
                _ActionState = ActionState::Completed;
            }
            else
            {
                _ActionState = ActionState::Ongoing;
            }
            break;

        case ActionState::Ongoing:
            if (RunAction())
            {
                _ActionState = ActionState::Completed;
            }
            break;

        default:
            break;
        }

        if (_ActionState == ActionState::Completed)
        {
            CleanupAction();
            _ActionState = ActionState::Idle;
            return true;
        }

        return false;
    }

    bool Action::InitAction()
    {
        return false;
    }

    bool Action::RunAction()
    {
        return true;
    }

    void Action::CleanupAction()
    {
    }

    FACTORY_DEFINE(Action);
}
FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);