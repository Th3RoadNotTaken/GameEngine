#pragma once
#include "FIEAGameEngine/Types.h"
#include "Content/Attributed.h"
#include "FIEAGameEngine/FactoryService.h"
#include "FIEAGameEngine/Action.h"

namespace Fiea::Engine::Actions
{
    class ActionList : public Action
    {
        RTTI_DECLARATIONS(ActionList, Action);

    public:
        ActionList();
        ActionList(const ActionList& other) = default;
        ActionList(ActionList&& other) noexcept = default;
        ActionList& operator=(const ActionList& other) = default;
        ActionList& operator=(ActionList&& other) noexcept = default;
        virtual ~ActionList() = default;

        virtual bool InitAction() override;
        virtual bool RunAction() override;
        virtual void CleanupAction() override;

        void AddAction(Action* action);
        bool RemoveAction(Action* action);

    protected:
        explicit ActionList(const Fiea::Engine::Content::ClassDefinition& other, const RTTI::IdType& idType);

    private:
        static Fiea::Engine::Content::ClassDefinition CreateDefinitions();
    };

    FACTORY_DECLARE(ActionList, Action);
}