#include "AttributeBox.h"

namespace Fiea::Engine::Tests::Support
{
    using namespace std::string_literals;

    RTTI_DEFINITIONS(AttributeBox);

    AttributeBox::AttributeBox() : Attributed(&CreateDefinitions, TypeIdInstance())
    {
    }

    bool AttributeBox::operator==(const AttributeBox& rhs) const
    {
        if (this == &rhs)
            return true;

        if (ExternalInteger != rhs.ExternalInteger ||
            ExternalFloat != rhs.ExternalFloat ||
            ExternalString != rhs.ExternalString)
            return false;

        if (ExternalPointer != nullptr && rhs.ExternalPointer != nullptr)
        {
            if (ExternalPointer->Data() != rhs.ExternalPointer->Data())
                return false;
        }

        for (size_t i = 0; i < ARRAY_SIZE; ++i)
        {
            if (ExternalIntegerArray[i] != rhs.ExternalIntegerArray[i] ||
                ExternalFloatArray[i] != rhs.ExternalFloatArray[i] ||
                ExternalStringArray[i] != rhs.ExternalStringArray[i])
                return false;

            if (ExternalPointerArray[i] != nullptr && rhs.ExternalPointerArray[i] != nullptr)
            {
                if (ExternalPointerArray[i]->Data() != rhs.ExternalPointerArray[i]->Data())
                    return false;
            }
        }

        return true;
    }

    // Scope overrides
    [[nodiscard]] AttributeBox* AttributeBox::Clone() const
    {
        return new AttributeBox(*this);
    }

    // RTTI overrides
    std::string AttributeBox::ToString() const
    {
        return "AttributeBox"s;
    }

    bool AttributeBox::Equals(const RTTI* rhs) const
    {
        // this would happen without the override, but included for clarity
        return Scope::Equals(rhs);

        // if AttributeBox had fields that are NOT tracked by Scope, then there would be additional checks here
    }

    ClassDefinition AttributeBox::CreateDefinitions()
    {
        return {
        { "DefIntVal", offsetof(AttributeBox, ExternalInteger), Datum::_DatumType::Int, 1 },
        { "DefStrVal", offsetof(AttributeBox, ExternalString), Datum::_DatumType::String, 1 },
        { "DefFloatVal", offsetof(AttributeBox, ExternalFloat), Datum::_DatumType::Float, 1 },
        { "DefFooVal", offsetof(AttributeBox, ExternalPointer), Datum::_DatumType::Pointer, 1 },

        { "DefIntArrVal", offsetof(AttributeBox, ExternalIntegerArray), Datum::_DatumType::Int, AttributeBox::ARRAY_SIZE },
        { "DefStrArrVal", offsetof(AttributeBox, ExternalStringArray), Datum::_DatumType::String, AttributeBox::ARRAY_SIZE },
        { "DefFloatArrVal", offsetof(AttributeBox, ExternalFloatArray), Datum::_DatumType::Float, AttributeBox::ARRAY_SIZE },
        { "DefFooArrVal", offsetof(AttributeBox, ExternalPointerArray), Datum::_DatumType::Pointer, AttributeBox::ARRAY_SIZE },

        { "ResizableIntArr", 0, Datum::_DatumType::Int, 3 }
        };
    }

    ClassDefinition AttributeBox::AppendAttributes(const ClassDefinition& def, const ClassDefinition& additionalDef)
    {
        ClassDefinition baseCpy = def;

        for (auto& signature : additionalDef)
        {
            baseCpy.push_back(signature);
        }
        return baseCpy;
    }

    AttributeBox::AttributeBox(const ClassDefinition& other, const RTTI::IdType& idType) : Attributed([other]() -> ClassDefinition {return AppendAttributes(CreateDefinitions(), other); }, idType)
    {
    }

    FACTORY_DEFINE(AttributeBox);
}
FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Content::Scope);