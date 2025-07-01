#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/Action.h"
#include "FIEAGameEngine/FactoryService.h"

namespace Fiea::Engine::Actions
{
    class ActionCondition : public Action
    {
        RTTI_DECLARATIONS(ActionCondition, Action);

    public:
        ActionCondition();
        ActionCondition(const ActionCondition& rhs) = default;
        ActionCondition(ActionCondition&& rhs) noexcept = default;
        ActionCondition& operator=(const ActionCondition& rhs) = default;
        ActionCondition& operator=(ActionCondition&& rhs) noexcept = default;
        ~ActionCondition() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        std::string _DatumKey;
        float _IncAmount;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
        bool _IncCondition;
    };

    FACTORY_DECLARE(ActionCondition, Action);
}