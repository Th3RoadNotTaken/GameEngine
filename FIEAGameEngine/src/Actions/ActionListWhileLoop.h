#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/ActionList.h"
#include "FIEAGameEngine/FactoryService.h"

namespace Fiea::Engine::Actions
{
    class ActionListWhileLoop : public ActionList
    {
        RTTI_DECLARATIONS(ActionListWhileLoop, ActionList);

    public:
        ActionListWhileLoop();
        ActionListWhileLoop(const ActionListWhileLoop& rhs) = default;
        ActionListWhileLoop(ActionListWhileLoop&& rhs) noexcept = default;
        ActionListWhileLoop& operator=(const ActionListWhileLoop& rhs) = default;
        ActionListWhileLoop& operator=(ActionListWhileLoop&& rhs) noexcept = default;
        ~ActionListWhileLoop() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        int32_t _Counter;
        int32_t _Limit;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
    };

    FACTORY_DECLARE(ActionListWhileLoop, Action);
}