#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/Action.h"
#include "FIEAGameEngine/FactoryService.h"
#include <chrono>

namespace Fiea::Engine::Actions
{
    class ActionAddDatum : public Action
    {
        RTTI_DECLARATIONS(ActionAddDatum, Action);

    public:
        ActionAddDatum();
        ActionAddDatum(const ActionAddDatum& rhs) = default;
        ActionAddDatum(ActionAddDatum&& rhs) noexcept = default;
        ActionAddDatum& operator=(const ActionAddDatum& rhs) = default;
        ActionAddDatum& operator=(ActionAddDatum&& rhs) noexcept = default;
        ~ActionAddDatum() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        std::string _DatumKey;
        std::string _DatumStr;
        float _RunTime;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
        std::chrono::steady_clock::time_point _StartTime;
    };

    FACTORY_DECLARE(ActionAddDatum, Action);
}