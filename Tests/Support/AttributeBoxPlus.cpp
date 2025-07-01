#include "AttributeBoxPlus.h"

namespace Fiea::Engine::Tests::Support
{
    using namespace std::string_literals;

    RTTI_DEFINITIONS(AttributeBoxPlus);

    AttributeBoxPlus::AttributeBoxPlus() : AttributeBox({{ "DerivedFloatVal", offsetof(AttributeBoxPlus, ExternalFloat2), Datum::_DatumType::Float, 1 }}, TypeIdInstance())
    {
    }

    bool AttributeBoxPlus::operator==(const AttributeBoxPlus& rhs) const
    {
        if (this == &rhs)
            return true;

        bool equal = AttributeBox::operator==(rhs);

        return equal && ExternalFloat2 == rhs.ExternalFloat2;
    }

    // Scope overrides
    [[nodiscard]] AttributeBoxPlus* AttributeBoxPlus::Clone() const
    {
        return new AttributeBoxPlus(*this);
    }

    // RTTI overrides
    std::string AttributeBoxPlus::ToString() const
    {
        return "AttributeBoxPlus"s;
    }

    bool AttributeBoxPlus::Equals(const RTTI* rhs) const
    {
        // this would happen without the override, but included for clarity
        return AttributeBox::Equals(rhs);

        // if AttributeBox had fields that are NOT tracked by Scope, then there would be additional checks here
    }
}