#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/ActionList.h"
#include "FIEAGameEngine/FactoryService.h"
#include <chrono>

namespace Fiea::Engine::Actions
{
    class ActionTimed : public ActionList
    {
        RTTI_DECLARATIONS(ActionTimed, ActionList);

    public:
        ActionTimed();
        ActionTimed(const ActionTimed& rhs) = default;
        ActionTimed(ActionTimed&& rhs) noexcept = default;
        ActionTimed& operator=(const ActionTimed& rhs) = default;
        ActionTimed& operator=(ActionTimed&& rhs) noexcept = default;
        ~ActionTimed() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        float _RunTime;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
        std::chrono::steady_clock::time_point _StartTime;
        bool firstRun;
    };

    FACTORY_DECLARE(ActionTimed, Action);
}