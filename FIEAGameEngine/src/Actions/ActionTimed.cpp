#include "ActionTimed.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionTimed);

    ClassDefinition ActionTimed::CreateDefinitions()
    {
        return {
            { "RunTime", offsetof(ActionTimed, _RunTime), Datum::_DatumType::Float, 1 }
        };
    }

    ActionTimed::ActionTimed() : ActionList(CreateDefinitions(), TypeIdInstance()), _RunTime(0.0f), firstRun(true)
    {
    }

    bool ActionTimed::InitAction()
    {
        _StartTime = std::chrono::steady_clock::now();
        return false;
    }

    bool ActionTimed::RunAction()
    {
        if (firstRun)
        {
            firstRun = false;
            _StartTime = std::chrono::steady_clock::now();
        }
        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - _StartTime).count();

        if (elapsed < _RunTime)
        {
            Datum* bodyDatum = Find("ActionList");

            if (bodyDatum != nullptr)
            {
                for (size_t i = bodyDatum->Size(); i-- > 0;)
                {
                    Action* nestedAction = bodyDatum->GetAsTable(i)->As<Action>();
                    FIEA_ERROR(nestedAction != nullptr);
                    bool actionCompleted = nestedAction->Update();
                    if (actionCompleted)
                    {
                        bodyDatum->RemoveAt(i);
                        delete nestedAction;
                    }
                }
            }
            return false;
        }
        else
        {
            _StartTime = std::chrono::steady_clock::now();
            return true;
        }
    }

    void ActionTimed::CleanupAction()
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

    FACTORY_DEFINE(ActionTimed);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);