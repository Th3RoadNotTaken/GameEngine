#pragma once
#include "FieaGameEngine/Types.h"
#include "FIEAGameEngine/Action.h"
#include "FIEAGameEngine/FactoryService.h"
#include <chrono>

namespace Fiea::Engine::Actions
{
    class ActionOutput : public Action
    {
        RTTI_DECLARATIONS(ActionOutput, Action);

    public:
        ActionOutput();
        ActionOutput(const ActionOutput& rhs) = default;
        ActionOutput(ActionOutput&& rhs) noexcept = default;
        ActionOutput& operator=(const ActionOutput& rhs) = default;
        ActionOutput& operator=(ActionOutput&& rhs) noexcept = default;
        ~ActionOutput() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        std::string _DatumKey;
        float _Delay;
        uint32_t _RepeatCount;

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
        std::chrono::steady_clock::time_point _StartTime;
        bool _InfiniteRun;
    };

    FACTORY_DECLARE(ActionOutput, Action);
}