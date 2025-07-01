#include "Foo.h"

namespace Fiea::Engine::Tests::Support
{
    RTTI_DEFINITIONS(Foo);

    Foo::Foo(int32_t data) : _Data(new int32_t(data))
    {
    }

    Foo::Foo(const Foo& rhs) : _Data(new int32_t(*rhs._Data))
    {

    }

    Foo::Foo(Foo&& rhs) noexcept : _Data(rhs._Data)
    {
        rhs._Data = nullptr;
    }

    Foo& Foo::operator=(const Foo& rhs)
    {
        if (this != &rhs)
        {
            *_Data = *rhs._Data;
        }
        return *this;
    }

    Foo& Foo::operator=(Foo&& rhs) noexcept
    {
        if (this != &rhs)
        {
            delete _Data;

            _Data = rhs._Data;
            rhs._Data = nullptr;
        }
        return *this;
    }

    Foo::~Foo()
    {
        delete _Data;
        _Data = NULL;
    }

    bool Foo::operator==(const Foo& rhs) const
    {
        return *_Data == *rhs._Data;
    }

    bool Foo::operator!=(const Foo& rhs) const
    {
        return !operator==(rhs);
    }

    /// @brief Retrieves a copy of the data held by this foo
    /// @return A copy of the data
    int32_t Foo::Data() const
    {
        return *_Data;
    }

    /// @brief Sets the data within this Foo
    /// @param data The data to set
    void Foo::SetData(int32_t data)
    {
        *_Data = data;
    }

    std::string Foo::ToString() const
    {
        std::ostringstream ostr;
        ostr << TypeName() << "(" << Data() << ")";
        return ostr.str();
    }

    bool Foo::Equals(const RTTI* rhs) const
    {
        const Foo* other = rhs->As<Foo>();
        if (other == nullptr) return false;
        return *_Data == *other->_Data;
    }
}