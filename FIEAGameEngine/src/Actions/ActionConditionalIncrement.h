#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/Action.h"
#include "FIEAGameEngine/FactoryService.h"

namespace Fiea::Engine::Actions
{
    class ActionConditionalIncrement : public Action
    {
        RTTI_DECLARATIONS(ActionConditionalIncrement, Action);

    public:
        ActionConditionalIncrement();
        ActionConditionalIncrement(const ActionConditionalIncrement& rhs) = default;
        ActionConditionalIncrement(ActionConditionalIncrement&& rhs) noexcept = default;
        ActionConditionalIncrement& operator=(const ActionConditionalIncrement& rhs) = default;
        ActionConditionalIncrement& operator=(ActionConditionalIncrement&& rhs) noexcept = default;
        ~ActionConditionalIncrement() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        std::string _DatumKey;
        float _IncAmount;
        float _MaxVal;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
    };

    FACTORY_DECLARE(ActionConditionalIncrement, Action);
}