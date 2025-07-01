#pragma once
#include "Empty.h"
#include "FIEAGameEngine/RTTI.h"
#include "sstream"

namespace Fiea::Engine::Tests::Support
{
    class EmptyChild : public Empty
    {
        RTTI_DECLARATIONS(EmptyChild, Empty);

    private:
        int32_t _Value;

    public:
        EmptyChild() = delete;
        EmptyChild(const EmptyChild&) = delete;
        EmptyChild& operator=(const EmptyChild&) = delete;

        EmptyChild(int32_t val) { _Value = val; }
        virtual int32_t GetValue() const override
        {
            return _Value;
        }

        std::string ToString() const override
        {
            std::ostringstream ostr;
            ostr << TypeName() << "(" << _Value << ")";
            return ostr.str();
        }
        bool Equals(const RTTI* rhs) const override
        {
            const EmptyChild* other = rhs->As<EmptyChild>();
            if (other == nullptr) return false;
            return _Value == other->_Value;
        }
    };

    RTTI_DEFINITIONS(EmptyChild);
}