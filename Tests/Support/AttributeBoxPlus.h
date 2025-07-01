#pragma once
#include "FieaGameEngine/Types.h"
#include "Content/Scope.h"
#include "../Tests.h"
#include "Foo.h"
#include "AttributeBox.h"
#include <cstddef>

namespace Fiea::Engine::Tests::Support
{
    using namespace Content;

    class AttributeBoxPlus : public AttributeBox
    {
        RTTI_DECLARATIONS(AttributeBoxPlus, AttributeBox);

    public:
        static const std::size_t ARRAY_SIZE = 5;

        // Rule-of-5 methods
        AttributeBoxPlus();
        AttributeBoxPlus(const AttributeBoxPlus& rhs) = default;
        AttributeBoxPlus(AttributeBoxPlus&& rhs) noexcept = default;
        AttributeBoxPlus& operator=(const AttributeBoxPlus& rhs) = default;
        AttributeBoxPlus& operator=(AttributeBoxPlus&& rhs) noexcept = default;
        virtual ~AttributeBoxPlus() = default;

        bool operator==(const AttributeBoxPlus& rhs) const;

        // Scope overrides
        [[nodiscard]] AttributeBoxPlus* Clone() const override;

        // RTTI overrides
        virtual std::string ToString() const override;
        virtual bool Equals(const RTTI* rhs) const override;

        float ExternalFloat2{};
    };
}

// ToString specializations required for Are*Equal / Are*Same
#include <CppUnitTest.h>
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    using Fiea::Engine::Tests::Support::AttributeBoxPlus;

    template<>
    inline std::wstring ToString<AttributeBoxPlus>(const AttributeBoxPlus& t)
    {
        RETURN_WIDE_STRING(t.ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBoxPlus>(const AttributeBoxPlus* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }

    template<>
    inline std::wstring ToString<AttributeBoxPlus>(AttributeBoxPlus* t)
    {
        RETURN_WIDE_STRING(t->ToString().c_str());
    }
}