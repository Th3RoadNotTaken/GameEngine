#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/Action.h"
#include "FIEAGameEngine/FactoryService.h"

namespace Fiea::Engine::Actions
{
    class ActionIncrement : public Action
    {
        RTTI_DECLARATIONS(ActionIncrement, Action);

    public:
        ActionIncrement();
        ActionIncrement(const ActionIncrement& rhs) = default;
        ActionIncrement(ActionIncrement&& rhs) noexcept = default;
        ActionIncrement& operator=(const ActionIncrement& rhs) = default;
        ActionIncrement& operator=(ActionIncrement&& rhs) noexcept = default;
        ~ActionIncrement() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        std::string _DatumKey;
        float _IncAmount;
        float _MaxVal;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
    };

    FACTORY_DECLARE(ActionIncrement, Action);
}