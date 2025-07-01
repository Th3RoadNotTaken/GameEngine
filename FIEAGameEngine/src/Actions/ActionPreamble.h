#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/Action.h"
#include "FIEAGameEngine/FactoryService.h"

namespace Fiea::Engine::Actions
{
    class ActionPreamble : public Action
    {
        RTTI_DECLARATIONS(ActionPreamble, Action);

    public:
        ActionPreamble();
        ActionPreamble(const ActionPreamble& rhs) = default;
        ActionPreamble(ActionPreamble&& rhs) noexcept = default;
        ActionPreamble& operator=(const ActionPreamble& rhs) = default;
        ActionPreamble& operator=(ActionPreamble&& rhs) noexcept = default;
        ~ActionPreamble() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        std::string _DatumKey;
        float _IncAmount;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
    };

    FACTORY_DECLARE(ActionPreamble, Action);
}