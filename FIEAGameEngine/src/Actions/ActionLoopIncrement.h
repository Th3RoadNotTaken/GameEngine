#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/Action.h"
#include "FIEAGameEngine/FactoryService.h"

namespace Fiea::Engine::Actions
{
    class ActionLoopIncrement : public Action
    {
        RTTI_DECLARATIONS(ActionLoopIncrement, Action);

    public:
        ActionLoopIncrement();
        ActionLoopIncrement(const ActionLoopIncrement& rhs) = default;
        ActionLoopIncrement(ActionLoopIncrement&& rhs) noexcept = default;
        ActionLoopIncrement& operator=(const ActionLoopIncrement& rhs) = default;
        ActionLoopIncrement& operator=(ActionLoopIncrement&& rhs) noexcept = default;
        ~ActionLoopIncrement() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        int32_t _IncAmount;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
    };

    FACTORY_DECLARE(ActionLoopIncrement, Action);
}