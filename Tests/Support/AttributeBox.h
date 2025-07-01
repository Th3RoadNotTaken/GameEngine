#pragma once
#include "FieaGameEngine/Types.h"
#include "Content/Scope.h"
#include "../Tests.h"
#include "Foo.h"
#include "Content/Attributed.h"
#include "FIEAGameEngine/FactoryService.h"
#include <cstddef>

namespace Fiea::Engine::Tests::Support
{
    using namespace Content;

    class AttributeBox : public Attributed
    {
        RTTI_DECLARATIONS(AttributeBox, Scope);

    public:
        static const std::size_t ARRAY_SIZE = 5;

        // Rule-of-5 methods
        AttributeBox();
        AttributeBox(const AttributeBox& rhs) = default;
        AttributeBox(AttributeBox&& rhs) noexcept = default;
        AttributeBox& operator=(const AttributeBox& rhs) = default;
        AttributeBox& operator=(AttributeBox&& rhs) noexcept = default;
        virtual ~AttributeBox() = default;

        bool operator==(const AttributeBox& rhs) const;

        // Scope overrides
        [[nodiscard]] AttributeBox* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        int32_t ExternalInteger{};
        float ExternalFloat{};
        string ExternalString{};
        Foo* ExternalPointer{};

        int32_t ExternalIntegerArray[ARRAY_SIZE]{};
        float ExternalFloatArray[ARRAY_SIZE]{};
        string ExternalStringArray[ARRAY_SIZE]{};
        Foo* ExternalPointerArray[ARRAY_SIZE]{}; 

    protected:
        explicit AttributeBox(const ClassDefinition& other, const RTTI::IdType& idType);

    private:
        static ClassDefinition CreateDefinitions();
        static ClassDefinition AppendAttributes(const ClassDefinition& def, const ClassDefinition& additionalDef);
    };

    FACTORY_DECLARE(AttributeBox, Fiea::Engine::Content::Scope);
}

// ToString specializations required for Are*Equal / Are*Same
#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    using Fiea::Engine::Tests::Support::AttributeBox;

    template<>
    inline std::wstring ToString<AttributeBox>(const AttributeBox& t)
    {
        RETURN_WIDE_STRING(t.ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBox>(const AttributeBox* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBox>(AttributeBox* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }
}