#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/ActionList.h"
#include "FIEAGameEngine/FactoryService.h"
#include <chrono>

namespace Fiea::Engine::Actions
{
    class ActionDelayed : public ActionList
    {
        RTTI_DECLARATIONS(ActionDelayed, ActionList);

    public:
        ActionDelayed();
        ActionDelayed(const ActionDelayed& rhs) = default;
        ActionDelayed(ActionDelayed&& rhs) noexcept = default;
        ActionDelayed& operator=(const ActionDelayed& rhs) = default;
        ActionDelayed& operator=(ActionDelayed&& rhs) noexcept = default;
        ~ActionDelayed() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        float _Delay;
        uint32_t _RepeatCount;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
        std::chrono::steady_clock::time_point _StartTime;
        bool _InfiniteRun;
        std::vector<int8_t> ActiveActions;
    };

    FACTORY_DECLARE(ActionDelayed, Action);
}