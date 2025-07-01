#include "ActionDelayed.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionDelayed);

    ClassDefinition ActionDelayed::CreateDefinitions()
    {
        return {
            { "Delay", offsetof(ActionDelayed, _Delay), Datum::_DatumType::Float, 1 },
            { "RepeatCount", offsetof(ActionDelayed, _RepeatCount), Datum::_DatumType::Int, 1 }
        };
    }

    ActionDelayed::ActionDelayed() : ActionList(CreateDefinitions(), TypeIdInstance()), _Delay(0.0f), _InfiniteRun(false), _RepeatCount(1)
    {
    }

    bool ActionDelayed::InitAction()
    {
        _StartTime = std::chrono::steady_clock::now();
        if (_RepeatCount == 0)
            _InfiniteRun = true;

        Datum* bodyDatum = Find("ActionList");
        if (bodyDatum != nullptr)
        {
            ActiveActions.resize(bodyDatum->Size());
            for (size_t i = 0; i < ActiveActions.size(); ++i)
                ActiveActions[i] = 1;
        }
        return false;
    }

    bool ActionDelayed::RunAction()
    {
        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - _StartTime).count();

        if (elapsed < _Delay)
            return false;

        Datum* bodyDatum = Find("ActionList");
        bool allCompleted = true;

        if (bodyDatum != nullptr)
        {
            for (size_t i = 0; i < bodyDatum->Size(); ++i)
            {
                Action* nestedAction = bodyDatum->GetAsTable(i)->As<Action>();
                FIEA_ERROR(nestedAction != nullptr);

                if (ActiveActions[i] == 1)
                {
                    bool actionCompleted = nestedAction->Update();
                    if (actionCompleted)
                        ActiveActions[i] = 0;
                    else
                        allCompleted = false;
                }
            }
        }

        if (allCompleted == true)
        {
            if (!_InfiniteRun)
            {
                for (size_t i = 0; i < ActiveActions.size(); ++i)
                    ActiveActions[i] = 1;

                --_RepeatCount;
            }

            if (!_InfiniteRun && _RepeatCount == 0)
                return true;

            _StartTime = std::chrono::steady_clock::now();
        }
        return false;
    }

    void ActionDelayed::CleanupAction()
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

    FACTORY_DEFINE(ActionDelayed);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);