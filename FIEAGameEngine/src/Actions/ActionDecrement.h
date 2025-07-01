#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/Action.h"
#include "FIEAGameEngine/FactoryService.h"

namespace Fiea::Engine::Actions
{
    class ActionDecrement : public Action
    {
        RTTI_DECLARATIONS(ActionDecrement, Action);

    public:
        ActionDecrement();
        ActionDecrement(const ActionDecrement& rhs) = default;
        ActionDecrement(ActionDecrement&& rhs) noexcept = default;
        ActionDecrement& operator=(const ActionDecrement& rhs) = default;
        ActionDecrement& operator=(ActionDecrement&& rhs) noexcept = default;
        ~ActionDecrement() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        std::string _DatumKey;
        float _DecAmount;
        float _MinVal;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
    };

    FACTORY_DECLARE(ActionDecrement, Action);
}