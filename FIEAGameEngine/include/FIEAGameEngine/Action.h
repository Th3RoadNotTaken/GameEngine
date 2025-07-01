#pragma once
#include "FIEAGameEngine/Types.h"
#include "Content/Attributed.h"
#include "FIEAGameEngine/FactoryService.h"
#include <functional>

namespace Fiea::Engine::Actions
{
    class Action : public Fiea::Engine::Content::Attributed
    {
        RTTI_DECLARATIONS(Action, Attributed);

    public:
        enum ActionState
        {
            Idle,
            Ongoing,
            Completed
        };

        Action();
        Action(const Action& other);
        Action(Action&& other) noexcept;
        Action& operator=(const Action& other);
        Action& operator=(Action&& other) noexcept;
        virtual ~Action() = default;

        bool Update();
        virtual bool InitAction();
        virtual bool RunAction();
        virtual void CleanupAction();

        std::string _ActionName;
        ActionState _ActionState;

    protected:
        explicit Action(const Fiea::Engine::Content::ClassDefinition& other, const RTTI::IdType& idType);

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
        static Fiea::Engine::Content::ClassDefinition AppendAttributes(const Fiea::Engine::Content::ClassDefinition& def, const Fiea::Engine::Content::ClassDefinition& additionalDef);
    };

    FACTORY_DECLARE(Action, Action);
}