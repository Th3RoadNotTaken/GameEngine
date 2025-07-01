#include "ActionOutput.h"
#include "../Tests.h"

namespace Fiea::Engine::Actions
{
    using namespace Fiea::Engine::Content;

    RTTI_DEFINITIONS(ActionOutput);

    ClassDefinition ActionOutput::CreateDefinitions()
    {
        return {
            { "OutputDatum", offsetof(ActionOutput, _DatumKey), Datum::_DatumType::String, 1 },
            { "Delay", offsetof(ActionOutput, _Delay), Datum::_DatumType::Float, 1 },
            { "RepeatCount", offsetof(ActionOutput, _RepeatCount), Datum::_DatumType::Int, 1 }
        };
    }

    ActionOutput::ActionOutput() : Action(CreateDefinitions(), TypeIdInstance()), _Delay(0), _RepeatCount(1)
    {
    }

    bool ActionOutput::InitAction()
    {
        _StartTime = std::chrono::steady_clock::now();
        if (_RepeatCount == 0)
            _InfiniteRun = true;

        return false;
    }

    bool ActionOutput::RunAction()
    {
        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration_cast<std::chrono::duration<float>>(now - _StartTime).count();

        if (elapsed < _Delay)
            return false;

        Datum* outputDatum = Search(_DatumKey.c_str());
        if (outputDatum != nullptr)
        {
            LOG(outputDatum->GetToString(0) + "\n");
        }

        if (!_InfiniteRun && _RepeatCount == 0)
            return true;

        _StartTime = std::chrono::steady_clock::now();
        if (!_InfiniteRun)
            --_RepeatCount;

        return false;
    }

    void ActionOutput::CleanupAction()
    {
    }

    FACTORY_DEFINE(ActionOutput);
}

FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Actions::Action);