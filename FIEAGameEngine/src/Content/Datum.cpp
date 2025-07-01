#include "Content/Datum.h"
#include "Content/Scope.h"

namespace Fiea::Engine::Content
{
    Datum::Datum() : _Type(_DatumType::Unknown), _Size(0), _Capacity(0), _DataPointer(nullptr), _IsExternal(false)
    {
        _Data._Int = nullptr;
    }

    Datum::Datum(int32_t value) : _Type(_DatumType::Int), _Size(1), _Capacity(1), _DataPointer(nullptr), _IsExternal(false)
    {
        _Data._Int = static_cast<int32_t*>(::operator new(sizeof(int32_t)));
        new (_Data._Int) int32_t(value);
        _Data._Int[0] = value;
    }

    Datum::Datum(float value) : _Type(_DatumType::Float), _Size(1), _Capacity(1), _DataPointer(nullptr), _IsExternal(false)
    {
        _Data._Float = static_cast<float*>(::operator new(sizeof(float)));
        new (_Data._Float) float(value);
        _Data._Float[0] = value;
    }

    Datum::Datum(string value) : _Type(_DatumType::String), _Size(1), _Capacity(1), _DataPointer(nullptr), _IsExternal(false)
    {
        _Data._String = static_cast<string*>(::operator new(sizeof(string)));
        new (_Data._String) string(value);
        _Data._String[0] = value;
    }

    Datum::Datum(RTTI* value) : _Type(_DatumType::Pointer), _Size(1), _Capacity(1), _DataPointer(nullptr), _IsExternal(false)
    {
        _Data._Pointer = static_cast<RTTI**>(::operator new(sizeof(RTTI*)));
        new (_Data._Pointer) RTTI* (value);
        _Data._Pointer[0] = value;
    }

    Datum::Datum(Scope* value) : _Type(_DatumType::Table), _Size(1), _Capacity(1), _DataPointer(nullptr), _IsExternal(false)
    {
        _Data._Table = static_cast<Scope**>(::operator new(sizeof(Scope*)));
        new (_Data._Table) Scope* (value);
        _Data._Table[0] = value;
    }

    Datum::Datum(Vec4 value)
    {
        _Data._Vec4 = static_cast<Vec4*>(::operator new(sizeof(Vec4)));
        new (_Data._Vec4) Vec4(value);
        _Data._Vec4[0] = value;
    }

    Datum::Datum(Mat4x4 value)
    {
        _Data._Mat4x4 = static_cast<Mat4x4*>(::operator new(sizeof(Mat4x4)));
        new (_Data._Mat4x4) Mat4x4(value);
        _Data._Mat4x4[0] = value;
    }

    Datum::Datum(std::initializer_list<int32_t> values)
    {
        FIEA_ERROR(values.size() > 0);

        _Data._Int = static_cast<std::int32_t*>(::operator new(sizeof(int32_t) * values.size()));
        for (size_t i = 0; i < values.size(); i++)
        {
            new (_Data._Int + i) int32_t(_Data._Int[i]);
        }

        size_t idx = 0;
        for (const auto& value : values)
        {
            _Data._Int[idx++] = value;
        }
        _Type = _DatumType::Int;
        _Size = idx;
        _Capacity = idx;
        _DataPointer = nullptr;
        _IsExternal = false;
    }

    Datum::Datum(std::initializer_list<float> values) : _Type(_DatumType::Float), _Size(values.size()), _Capacity(values.size())
    {
        FIEA_ERROR(values.size() > 0);

        _Data._Float = static_cast<float*>(::operator new(sizeof(float) * values.size()));
        for (size_t i = 0; i < values.size(); i++)
        {
            new (_Data._Float + i) float(_Data._Float[i]);
        }

        size_t idx = 0;
        for (const auto& value : values)
        {
            _Data._Float[idx++] = value;
        }
        _Type = _DatumType::Float;
        _Size = idx;
        _Capacity = idx;
        _DataPointer = nullptr;
        _IsExternal = false;
    }

    Datum::Datum(std::initializer_list<string> values) : _Type(_DatumType::String), _Size(values.size()), _Capacity(values.size())
    {
        FIEA_ERROR(values.size() > 0);

        _Data._String = static_cast<string*>(::operator new(sizeof(string) * values.size()));
        for (size_t i = 0; i < values.size(); i++)
        {
            new (_Data._String + i) string(_Data._String[i]);
        }
        
        size_t idx = 0;
        for (const auto& value : values)
        {
            _Data._String[idx++] = value;
        }
        _Type = _DatumType::String;
        _Size = idx;
        _Capacity = idx;
        _DataPointer = nullptr;
        _IsExternal = false;
    }

    Datum::Datum(std::initializer_list<RTTI*> values) : _Type(_DatumType::Pointer), _Size(values.size()), _Capacity(values.size())
    {
        FIEA_ERROR(values.size() > 0);

        _Data._Pointer = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * values.size()));
        for (size_t i = 0; i < values.size(); i++)
        {
            new (_Data._Pointer + i) RTTI*(_Data._Pointer[i]);
        }

        size_t idx = 0;
        for (const auto& value : values)
        {
            _Data._Pointer[idx++] = value;
        }
        _Type = _DatumType::Pointer;
        _Size = idx;
        _Capacity = idx;
        _DataPointer = nullptr;
        _IsExternal = false;
    }

    Datum::Datum(std::initializer_list<Scope*> values) : _Type(_DatumType::Table), _Size(values.size()), _Capacity(values.size())
    {
        FIEA_ERROR(values.size() > 0);

        _Data._Table = static_cast<Scope**>(::operator new(sizeof(Scope*) * values.size()));
        for (size_t i = 0; i < values.size(); i++)
        {
            new (_Data._Table + i) Scope* (_Data._Table[i]);
        }

        size_t idx = 0;
        for (const auto& value : values)
        {
            _Data._Table[idx++] = value;
        }
        _Type = _DatumType::Table;
        _Size = idx;
        _Capacity = idx;
        _DataPointer = nullptr;
        _IsExternal = false;
    }

    Datum::Datum(std::initializer_list<Vec4> values) : _Type(_DatumType::Vec4), _Size(values.size()), _Capacity(values.size())
    {
        FIEA_ERROR(values.size() > 0);

        _Data._Vec4 = static_cast<Vec4*>(::operator new(sizeof(Vec4) * values.size()));
        for (size_t i = 0; i < values.size(); i++)
        {
            new (_Data._Vec4 + i) Vec4(_Data._Vec4[i]);
        }

        size_t idx = 0;
        for (const auto& value : values)
        {
            _Data._Vec4[idx++] = value;
        }
        _Type = _DatumType::Vec4;
        _Size = idx;
        _Capacity = idx;
        _DataPointer = nullptr;
        _IsExternal = false;
    }

    Datum::Datum(std::initializer_list<Mat4x4> values) : _Type(_DatumType::Mat4x4), _Size(values.size()), _Capacity(values.size())
    {
        FIEA_ERROR(values.size() > 0);

        _Data._Mat4x4 = static_cast<Mat4x4*>(::operator new(sizeof(Mat4x4) * values.size()));
        for (size_t i = 0; i < values.size(); i++)
        {
            new (_Data._Mat4x4 + i) Mat4x4(_Data._Mat4x4[i]);
        }

        size_t idx = 0;
        for (const auto& value : values)
        {
            _Data._Mat4x4[idx++] = value;
        }
        _Type = _DatumType::Mat4x4;
        _Size = idx;
        _Capacity = idx;
        _DataPointer = nullptr;
        _IsExternal = false;
    }

    Datum::~Datum()
    {
        if (!_IsExternal)
        {
            switch (_Type)
            {
            case _DatumType::Int:
                if (_Data._Int != nullptr)
                {
                    ::operator delete(_Data._Int);
                    _Data._Int = nullptr;
                }
                break;
            case _DatumType::Float:
                if (_Data._Float != nullptr)
                {
                    ::operator delete(_Data._Float);
                    _Data._Float = nullptr;
                }
                break;
            case _DatumType::String:
                if (_Data._String != nullptr)
                {
                    for (size_t i = 0; i < _Capacity; ++i)
                    {
                        _Data._String[i].~basic_string();
                    }
                    ::operator delete(_Data._String);
                    _Data._String = nullptr;
                }
                break;
            case _DatumType::Pointer:
                if (_Data._Pointer != nullptr)
                {
                    ::operator delete(_Data._Pointer);
                    _Data._Pointer = nullptr;
                }
                break;
            case _DatumType::Table:
                if (_Data._Table != nullptr)
                {
                    ::operator delete(_Data._Table);
                    _Data._Table = nullptr;
                }
                break;
            case _DatumType::Vec4:
                if (_Data._Vec4 != nullptr)
                {
                    ::operator delete(_Data._Vec4);
                    _Data._Vec4 = nullptr;
                }
                break;
            case _DatumType::Mat4x4:
                if (_Data._Mat4x4 != nullptr)
                {
                    ::operator delete(_Data._Mat4x4);
                    _Data._Mat4x4 = nullptr;
                }
                break;
            default:
                break;
            }
        }

        _Type = _DatumType::Unknown;
        _Size = 0;
        _Capacity = 0;
        _DataPointer = nullptr;
        _IsExternal = false;
    }

    Datum::Datum(const Datum& other) : _Type(other._Type), _Size(other._Size), _Capacity(other._Capacity), _IsExternal(other._IsExternal)
    {
        FIEA_ERROR(other._Capacity > 0 && other._Capacity >= other._Size);

        if (other._IsExternal == true)
        {
            _DataPointer = other._DataPointer;
            _Data._Int = nullptr;
        }
        else
        {
            _DataPointer = other._DataPointer;
            switch (other._Type)
            {
            case _DatumType::Int:
                FIEA_ERROR(other._Data._Int != nullptr);

                _Data._Int = static_cast<int32_t*>(::operator new(sizeof(int32_t) * other._Capacity));
                for (size_t i = 0; i < other._Capacity; i++)
                {
                    new (_Data._Int + i) int32_t(_Data._Int[i]);
                }

                for (size_t i = 0; i < other._Size; ++i)
                {
                    _Data._Int[i] = other._Data._Int[i];
                }
                break;
            case _DatumType::Float:
                FIEA_ERROR(other._Data._Float != nullptr);

                _Data._Float = static_cast<float*>(::operator new(sizeof(float) * other._Capacity));
                for (size_t i = 0; i < other._Capacity; i++)
                {
                    new (_Data._Float + i) float(_Data._Float[i]);
                }

                for (size_t i = 0; i < other._Size; ++i)
                {
                    _Data._Float[i] = other._Data._Float[i];
                }
                break;
            case _DatumType::String:
                FIEA_ERROR(other._Data._String != nullptr);

                _Data._String = static_cast<string*>(::operator new(sizeof(string) * other._Capacity));
                for (size_t i = 0; i < other._Capacity; i++)
                {
                    new (_Data._String + i) string(_Data._String[i]);
                }

                for (size_t i = 0; i < other._Size; ++i)
                {
                    _Data._String[i] = other._Data._String[i];
                }
                break;
            case _DatumType::Pointer:
                FIEA_ERROR(other._Data._Pointer != nullptr);

                _Data._Pointer = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * other._Capacity));
                for (size_t i = 0; i < other._Capacity; i++)
                {
                    new (_Data._Pointer + i) RTTI*(_Data._Pointer[i]);
                }

                for (size_t i = 0; i < other._Size; ++i)
                {
                    _Data._Pointer[i] = other._Data._Pointer[i];
                }
                break;
            case _DatumType::Table:
                FIEA_ERROR(other._Data._Table != nullptr);

                _Data._Table = static_cast<Scope**>(::operator new(sizeof(Scope*) * other._Capacity));
                for (size_t i = 0; i < other._Capacity; i++)
                {
                    new (_Data._Table + i) Scope* (_Data._Table[i]);
                }

                for (size_t i = 0; i < other._Size; ++i)
                {
                    _Data._Table[i] = other._Data._Table[i];
                }
                break;
            case _DatumType::Vec4:
                FIEA_ERROR(other._Data._Vec4 != nullptr);

                _Data._Vec4 = static_cast<Vec4*>(::operator new(sizeof(Vec4) * other._Capacity));
                for (size_t i = 0; i < other._Capacity; i++)
                {
                    new (_Data._Vec4 + i) Vec4(_Data._Vec4[i]);
                }

                for (size_t i = 0; i < other._Size; ++i)
                {
                    _Data._Vec4[i] = other._Data._Vec4[i];
                }
                break;
            case _DatumType::Mat4x4:
                FIEA_ERROR(other._Data._Mat4x4 != nullptr);

                _Data._Mat4x4 = static_cast<Mat4x4*>(::operator new(sizeof(Mat4x4) * other._Capacity));
                for (size_t i = 0; i < other._Capacity; i++)
                {
                    new (_Data._Mat4x4 + i) Mat4x4(_Data._Mat4x4[i]);
                }

                for (size_t i = 0; i < other._Size; ++i)
                {
                    _Data._Mat4x4[i] = other._Data._Mat4x4[i];
                }
                break;
            default:
                _Data._Pointer = nullptr;
                break;
            }
        }
    }

    Datum& Datum::operator=(const Datum& rhs)
    {
        if (this == &rhs)
            return *this;

        if (rhs._IsExternal)
        {
            _Type = rhs._Type;
            _Size = rhs._Size;
            _Capacity = rhs._Capacity;
            _IsExternal = true;
            _DataPointer = rhs._DataPointer;
            return *this;
        }

        switch (_Type)
        {
        case _DatumType::Int:
            if (_Data._Int != nullptr)
            {
                ::operator delete(_Data._Int);
                _Data._Int = nullptr;
            }
            break;

        case _DatumType::Float:
            if (_Data._Float != nullptr)
            {
                ::operator delete(_Data._Float);
                _Data._Float = nullptr;
            }
            break;

        case _DatumType::String:
            if (_Data._String != nullptr)
            {
                for (size_t i = 0; i < _Size; ++i)
                {
                    _Data._String[i].~basic_string();
                }
                ::operator delete(_Data._String);
                _Data._String = nullptr;
            }
            break;
        case _DatumType::Pointer:
            if (_Data._Pointer != nullptr)
            {
                ::operator delete(_Data._Pointer);
                _Data._Pointer = nullptr;
            }
            break;
        case _DatumType::Table:
            if (_Data._Table != nullptr)
            {
                ::operator delete(_Data._Table);
                _Data._Table = nullptr;
            }
            break;
        case _DatumType::Vec4:
            if (_Data._Vec4 != nullptr)
            {
                ::operator delete(_Data._Vec4);
                _Data._Vec4 = nullptr;
            }
            break;
        case _DatumType::Mat4x4:
            if (_Data._Mat4x4 != nullptr)
            {
                ::operator delete(_Data._Mat4x4);
                _Data._Mat4x4 = nullptr;
            }
            break;
        default:
            break;
        }

        _Type = _DatumType::Unknown;
        _Size = 0;

        FIEA_ERROR(_Type == _DatumType::Unknown || _Type == rhs._Type);

        _Type = _Type == _DatumType::Unknown ? rhs._Type : _Type;
        _Capacity = _Capacity >= rhs._Capacity ? _Capacity : rhs._Capacity;
        _Size = rhs._Size;

        FIEA_ERROR(rhs._Capacity > 0 && rhs._Capacity >= rhs._Size);

        switch (rhs._Type)
        {
        case _DatumType::Int:
            FIEA_ERROR(rhs._Data._Int != nullptr);

            _Data._Int = static_cast<int32_t*>(::operator new(sizeof(int32_t) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (_Data._Int + i) int32_t(_Data._Int[i]);
            }

            for (size_t i = 0; i < _Size; ++i)
            {
                _Data._Int[i] = rhs._Data._Int[i];
            }
            break;
        case _DatumType::Float:
            FIEA_ERROR(rhs._Data._Float != nullptr);

            _Data._Float = static_cast<float*>(::operator new(sizeof(float) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (_Data._Float + i) float(_Data._Float[i]);
            }

            for (size_t i = 0; i < _Size; ++i)
            {
                _Data._Float[i] = rhs._Data._Float[i];
            }
            break;
        case _DatumType::String:
            FIEA_ERROR(rhs._Data._String != nullptr);

            _Data._String = static_cast<string*>(::operator new(sizeof(string) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (_Data._String + i) string(_Data._String[i]);
            }

            for (size_t i = 0; i < _Size; ++i)
            {
                _Data._String[i] = rhs._Data._String[i];
            }
            break;
        case _DatumType::Pointer:
            FIEA_ERROR(rhs._Data._Pointer != nullptr);

            _Data._Pointer = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (_Data._Pointer + i) RTTI*(_Data._Pointer[i]);
            }

            for (size_t i = 0; i < _Size; ++i)
            {
                _Data._Pointer[i] = rhs._Data._Pointer[i];
            }
            break;
        case _DatumType::Table:
            FIEA_ERROR(rhs._Data._Table != nullptr);

            _Data._Table = static_cast<Scope**>(::operator new(sizeof(Scope*) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (_Data._Table + i) Scope* (_Data._Table[i]);
            }

            for (size_t i = 0; i < _Size; ++i)
            {
                _Data._Table[i] = rhs._Data._Table[i];
            }
            break;
        case _DatumType::Vec4:
            FIEA_ERROR(rhs._Data._Vec4 != nullptr);

            _Data._Vec4 = static_cast<Vec4*>(::operator new(sizeof(Vec4) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (_Data._Vec4 + i) Vec4(_Data._Vec4[i]);
            }

            for (size_t i = 0; i < _Size; ++i)
            {
                _Data._Vec4[i] = rhs._Data._Vec4[i];
            }
            break;
        case _DatumType::Mat4x4:
            FIEA_ERROR(rhs._Data._Mat4x4 != nullptr);

            _Data._Mat4x4 = static_cast<Mat4x4*>(::operator new(sizeof(Mat4x4) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (_Data._Mat4x4 + i) Mat4x4(_Data._Mat4x4[i]);
            }

            for (size_t i = 0; i < _Size; ++i)
            {
                _Data._Mat4x4[i] = rhs._Data._Mat4x4[i];
            }
            break;
        default:
            _Data._Int = nullptr;
            break;
        }
        return *this;
    }

    Datum::Datum(Datum&& other) noexcept : _Type(other._Type), _Size(other._Size), _Capacity(other._Capacity), _IsExternal(other._IsExternal), _DataPointer(other._DataPointer), _Data(other._Data)
    {
        other._Size = 0;
        other._Capacity = 0;
        other._Type = _DatumType::Unknown;
        other._IsExternal = false;
    }

    Datum& Datum::operator=(Datum&& rhs)
    {
        FIEA_ERROR(!rhs._IsExternal);

        if (this == &rhs)
            return *this;

        switch (_Type)
        {
        case _DatumType::Int:
            if (_Data._Int != nullptr)
            {
                ::operator delete(_Data._Int);
                _Data._Int = nullptr;
            }
            break;

        case _DatumType::Float:
            if (_Data._Float != nullptr)
            {
                ::operator delete(_Data._Float);
                _Data._Float = nullptr;
            }
            break;

        case _DatumType::String:
            if (_Data._String != nullptr)
            {
                for (size_t i = 0; i < _Size; ++i)
                {
                    _Data._String[i].~basic_string();
                }
                ::operator delete(_Data._String);
                _Data._String = nullptr;
            }
            break;

        case _DatumType::Pointer:
            if (_Data._Pointer != nullptr)
            {
                ::operator delete(_Data._Pointer);
                _Data._Pointer = nullptr;
            }
            break;
        case _DatumType::Table:
            if (_Data._Table != nullptr)
            {
                ::operator delete(_Data._Table);
                _Data._Table = nullptr;
            }
            break;
        case _DatumType::Vec4:
            if (_Data._Vec4 != nullptr)
            {
                ::operator delete(_Data._Vec4);
                _Data._Vec4 = nullptr;
            }
            break;
        case _DatumType::Mat4x4:
            if (_Data._Mat4x4 != nullptr)
            {
                ::operator delete(_Data._Mat4x4);
                _Data._Mat4x4 = nullptr;
            }
            break;

        default:
            break;
        }

        _Type = rhs._Type;
        _Size = rhs._Size;
        _Capacity = rhs._Capacity;
        _IsExternal = rhs._IsExternal;
        _DataPointer = rhs._DataPointer;
        _Data = rhs._Data;

        rhs._Size = 0;
        rhs._Capacity = 0;
        rhs._Type = _DatumType::Unknown;
        rhs._IsExternal = false;
        
        return *this;
    }

    Datum& Datum::operator=(const int32_t value)
    {
        FIEA_ERROR(_Type == _DatumType::Int || _Type == _DatumType::Unknown);

        if (_Type == _DatumType::Unknown)
        {
            _Data._Int = static_cast<int32_t*>(::operator new(sizeof(int32_t)));
            new (_Data._Int) int32_t(value);

            _Data._Int[0] = value;
            _Size = 1;
            _Capacity = 1;
            _Type = _DatumType::Int;

            return *this;
        }
        else
        {
            _Data._Int[0] = value;
            _Size = 1;

            return *this;
        }
    }

    Datum& Datum::operator=(const float value)
    {
        FIEA_ERROR(_Type == _DatumType::Float || _Type == _DatumType::Unknown);
        if (_Type == _DatumType::Unknown)
        {
            _Data._Float = static_cast<float*>(::operator new(sizeof(float)));
            new (_Data._Float) float(value);

            _Data._Float[0] = value;
            _Size = 1;
            _Capacity = 1;
            _Type = _DatumType::Float;

            return *this;
        }
        else
        {
            _Data._Float[0] = value;
            _Size = 1;

            return *this;
        }
    }

    Datum& Datum::operator=(const string value)
    {
        FIEA_ERROR(_Type == _DatumType::String || _Type == _DatumType::Unknown);

        if (_Type == _DatumType::Unknown)
        {
            _Data._String = static_cast<string*>(::operator new(sizeof(string)));
            new (_Data._String) string(value);

            _Data._String[0] = value;
            _Size = 1;
            _Capacity = 1;
            _Type = _DatumType::String;

            return *this;
        }
        else
        {
            _Data._String[0] = value;
            _Size = 1;

            return *this;
        }
    }

    Datum& Datum::operator=(RTTI* const value)
    {
        FIEA_ERROR(_Type == _DatumType::Pointer || _Type == _DatumType::Unknown);

        if (_Type == _DatumType::Unknown)
        {
            _Data._Pointer = static_cast<RTTI**>(::operator new(sizeof(RTTI*)));
            new (_Data._Pointer) RTTI*(value);

            _Data._Pointer[0] = value;
            _Size = 1;
            _Capacity = 1;
            _Type = _DatumType::Pointer;

            return *this;
        }
        else
        {
            _Data._Pointer[0] = value;
            _Size = 1;

            return *this;
        }
    }

    Datum& Datum::operator=(Scope* const value)
    {
        FIEA_ERROR(_Type == _DatumType::Table || _Type == _DatumType::Unknown);

        if (_Type == _DatumType::Unknown)
        {
            _Data._Table = static_cast<Scope**>(::operator new(sizeof(Scope*)));
            new (_Data._Table) Scope* (value);

            _Data._Table[0] = value;
            _Size = 1;
            _Capacity = 1;
            _Type = _DatumType::Table;

            return *this;
        }
        else
        {
            _Data._Table[0] = value;
            _Size = 1;

            return *this;
        }
    }

    Datum& Datum::operator=(const Vec4 value)
    {
        FIEA_ERROR(_Type == _DatumType::Vec4 || _Type == _DatumType::Unknown);
        if (_Type == _DatumType::Unknown)
        {
            _Data._Vec4 = static_cast<Vec4*>(::operator new(sizeof(Vec4)));
            new (_Data._Vec4) Vec4(value);

            _Data._Vec4[0] = value;
            _Size = 1;
            _Capacity = 1;
            _Type = _DatumType::Vec4;

            return *this;
        }
        else
        {
            _Data._Vec4[0] = value;
            _Size = 1;

            return *this;
        }
    }

    Datum& Datum::operator=(const Mat4x4 value)
    {
        FIEA_ERROR(_Type == _DatumType::Mat4x4 || _Type == _DatumType::Unknown);
        if (_Type == _DatumType::Unknown)
        {
            _Data._Mat4x4 = static_cast<Mat4x4*>(::operator new(sizeof(Mat4x4)));
            new (_Data._Mat4x4) Mat4x4(value);

            _Data._Mat4x4[0] = value;
            _Size = 1;
            _Capacity = 1;
            _Type = _DatumType::Mat4x4;

            return *this;
        }
        else
        {
            _Data._Mat4x4[0] = value;
            _Size = 1;

            return *this;
        }
    }

    bool Datum::operator==(const Datum& other) const
    {
        if (_Size != other._Size || _Type != other._Type)
            return false;

        if (_Type == _DatumType::Unknown && other._Type == _DatumType::Unknown)
            return true;

        size_t idx = 0;

        switch (_Type)
        {
        case _DatumType::Int:
            while (idx < _Size)
            {
                if (_Data._Int[idx] != other._Data._Int[idx])
                    return false;
                ++idx;
            }
            break;
        case _DatumType::Float:
            while (idx < _Size)
            {
                if (_Data._Float[idx] != other._Data._Float[idx])
                    return false;
                ++idx;
            }
            break;
        case _DatumType::String:
            while (idx < _Size)
            {
                if (_Data._String[idx] != other._Data._String[idx])
                    return false;
                ++idx;
            }
            break;
        case _DatumType::Pointer:
            while (idx < _Size)
            {
                if (_Data._Pointer[idx] != other._Data._Pointer[idx])
                    return false;
                ++idx;
            }
            break;
        case _DatumType::Table:
            while (idx < _Size)
            {
                if (*_Data._Table[idx] != *other._Data._Table[idx])
                    return false;
                ++idx;
            }
            break;
        case _DatumType::Vec4:
            while (idx < _Size)
            {
                if (_Data._Vec4[idx] != other._Data._Vec4[idx])
                    return false;
                ++idx;
            }
            break;
        case _DatumType::Mat4x4:
            while (idx < _Size)
            {
                if (_Data._Mat4x4[idx] != other._Data._Mat4x4[idx])
                    return false;
                ++idx;
            }
            break;
        default:
            break;
        }

        return true;
    }

    bool Datum::operator!=(const Datum& other) const
    {
        if (_Size != other._Size || _Type != other._Type)
            return true;

        if (_Type == _DatumType::Unknown && other._Type == _DatumType::Unknown)
            return false;

        size_t idx = 0;

        switch (_Type)
        {
        case _DatumType::Int:
            while (idx < _Size)
            {
                if (_Data._Int[idx] != other._Data._Int[idx])
                    return true;
                ++idx;
            }
            break;
        case _DatumType::Float:
            while (idx < _Size)
            {
                if (_Data._Float[idx] != other._Data._Float[idx])
                    return true;
                ++idx;
            }
            break;
        case _DatumType::String:
            while (idx < _Size)
            {
                if (_Data._String[idx] != other._Data._String[idx])
                    return true;
                ++idx;
            }
            break;
        case _DatumType::Pointer:
            while (idx < _Size)
            {
                if (_Data._Pointer[idx] != other._Data._Pointer[idx])
                    return true;
                ++idx;
            }
            break;
        case _DatumType::Table:
            while (idx < _Size)
            {
                if (*_Data._Table[idx] != *other._Data._Table[idx])
                    return true;
                ++idx;
            }
            break;
        case _DatumType::Vec4:
            while (idx < _Size)
            {
                if (_Data._Vec4[idx] != other._Data._Vec4[idx])
                    return true;
                ++idx;
            }
            break;
        case _DatumType::Mat4x4:
            while (idx < _Size)
            {
                if (_Data._Mat4x4[idx] != other._Data._Mat4x4[idx])
                    return true;
                ++idx;
            }
            break;
        default:
            break;
        }

        return false;
    }

    void Datum::SetType(_DatumType typeToSet)
    {
        FIEA_ERROR(_Type == typeToSet || _Type == _DatumType::Unknown);
        _Type = typeToSet;
    }

    bool Datum::IsType(_DatumType typeToCheck) const
    {
        return _Type == typeToCheck;
    }

    int32_t& Datum::GetAsInt(size_t idx)
    {
        FIEA_ERROR(_Type == _DatumType::Int);
        FIEA_ERROR(idx < _Size);

        if (_IsExternal)
        {
            return static_cast<int32_t*>(_DataPointer)[idx];
        }
        return _Data._Int[idx];
    }

    float& Datum::GetAsFloat(size_t idx)
    {
        FIEA_ERROR(_Type == _DatumType::Float);
        FIEA_ERROR(idx < _Size);

        if (_IsExternal)
        {
            return static_cast<float*>(_DataPointer)[idx];
        }

        return _Data._Float[idx];
    }

    string& Datum::GetAsString(size_t idx)
    {
        FIEA_ERROR(_Type == _DatumType::String);
        FIEA_ERROR(idx < _Size);

        if (_IsExternal)
        {
            return static_cast<string*>(_DataPointer)[idx];
        }

        return _Data._String[idx];
    }

    RTTI*& Datum::GetAsPointer(size_t idx)
    {
        FIEA_ERROR(_Type == _DatumType::Pointer);
        FIEA_ERROR(idx < _Size);

        if (_IsExternal)
        {
            return static_cast<RTTI**>(_DataPointer)[idx];
        }

        return _Data._Pointer[idx];
    }

    Scope*& Datum::GetAsTable(size_t idx)
    {
        FIEA_ERROR(_Type == _DatumType::Table);
        FIEA_ERROR(idx < _Size);

        if (_IsExternal)
        {
            return static_cast<Scope**>(_DataPointer)[idx];
        }

        return _Data._Table[idx];
    }

    Vec4& Datum::GetAsVec4(size_t idx)
    {
        FIEA_ERROR(_Type == _DatumType::Vec4);
        FIEA_ERROR(idx < _Size);

        if (_IsExternal)
        {
            return static_cast<Vec4*>(_DataPointer)[idx];
        }

        return _Data._Vec4[idx];
    }

    Mat4x4& Datum::GetAsMat4x4(size_t idx)
    {
        FIEA_ERROR(_Type == _DatumType::Mat4x4);
        FIEA_ERROR(idx < _Size);

        if (_IsExternal)
        {
            return static_cast<Mat4x4*>(_DataPointer)[idx];
        }

        return _Data._Mat4x4[idx];
    }

    const int32_t& Datum::GetAsInt(size_t idx) const
    {
        FIEA_ERROR(_Type == _DatumType::Int);
        FIEA_ERROR(idx < _Size);

        return _Data._Int[idx];
    }

    const float& Datum::GetAsFloat(size_t idx) const
    {
        FIEA_ERROR(_Type == _DatumType::Float);
        FIEA_ERROR(idx < _Size);

        return _Data._Float[idx];
    }

    const string& Datum::GetAsString(size_t idx) const
    {
        FIEA_ERROR(_Type == _DatumType::String);
        FIEA_ERROR(idx < _Size);

        return _Data._String[idx];
    }

    RTTI* const& Datum::GetAsPointer(size_t idx) const
    {
        FIEA_ERROR(_Type == _DatumType::Pointer);
        FIEA_ERROR(idx < _Size);

        return _Data._Pointer[idx];
    }

    Scope* const& Datum::GetAsTable(size_t idx) const
    {
        FIEA_ERROR(_Type == _DatumType::Table);
        FIEA_ERROR(idx < _Size);

        return _Data._Table[idx];
    }

    const Vec4& Datum::GetAsVec4(size_t idx) const
    {
        FIEA_ERROR(_Type == _DatumType::Vec4);
        FIEA_ERROR(idx < _Size);

        return _Data._Vec4[idx];
    }

    const Mat4x4& Datum::GetAsMat4x4(size_t idx) const
    {
        FIEA_ERROR(_Type == _DatumType::Mat4x4);
        FIEA_ERROR(idx < _Size);

        return _Data._Mat4x4[idx];
    }

    void Datum::ResizeArray(const _DatumType& type)
    {
        if (_Capacity == 0)
        {
            _Capacity = 1;
        }

        _Capacity = static_cast<size_t>((_Capacity == 1 ? 2 : 1.5) * _Capacity);

        switch (type)
        {
        case _DatumType::Int:
        {
            int32_t* newStorage = static_cast<int32_t*>(::operator new(sizeof(int32_t) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (newStorage + i) int32_t(newStorage[i]);
            }

            for (size_t i = 0; i < _Size; i++)
            {
                newStorage[i] = _Data._Int[i];
            }

            ::operator delete(_Data._Int);
            _Data._Int = newStorage;
            break;
        }
        case _DatumType::Float:
        {
            float* newStorage = static_cast<float*>(::operator new(sizeof(float) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (newStorage + i) float(newStorage[i]);
            }

            for (size_t i = 0; i < _Size; i++)
            {
                newStorage[i] = _Data._Float[i];
            }

            ::operator delete(_Data._Float);
            _Data._Float = newStorage;
            break;
        }
        case _DatumType::String:
        {
            string* newStorage = static_cast<string*>(::operator new(sizeof(string) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (newStorage + i) string(newStorage[i]);
            }

            for (size_t i = 0; i < _Size; i++)
            {
                newStorage[i] = _Data._String[i];
            }

            for (size_t i = 0; i < _Size; ++i)
            {
                _Data._String[i].~basic_string(); 
            }
            ::operator delete(_Data._String);
            _Data._String = newStorage;
            break;
        }
        case _DatumType::Pointer:
        {
            RTTI** newStorage = static_cast<RTTI**>(::operator new(sizeof(RTTI*) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (newStorage + i) RTTI*(newStorage[i]);
            }

            for (size_t i = 0; i < _Size; i++)
            {
                newStorage[i] = _Data._Pointer[i];
            }

            ::operator delete(_Data._Pointer);
            _Data._Pointer = newStorage;
            break;
        }
        case _DatumType::Table:
        {
            Scope** newStorage = static_cast<Scope**>(::operator new(sizeof(Scope*) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (newStorage + i) Scope* (newStorage[i]);
            }

            for (size_t i = 0; i < _Size; i++)
            {
                newStorage[i] = _Data._Table[i];
            }

            ::operator delete(_Data._Table);
            _Data._Table = newStorage;
            break;
        }
        case _DatumType::Vec4:
        {
            Vec4* newStorage = static_cast<Vec4*>(::operator new(sizeof(Vec4) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (newStorage + i) Vec4(newStorage[i]);
            }

            for (size_t i = 0; i < _Size; i++)
            {
                newStorage[i] = _Data._Vec4[i];
            }

            ::operator delete(_Data._Vec4);
            _Data._Vec4 = newStorage;
            break;
        }
        case _DatumType::Mat4x4:
        {
            Mat4x4* newStorage = static_cast<Mat4x4*>(::operator new(sizeof(Mat4x4) * _Capacity));
            for (size_t i = 0; i < _Capacity; i++)
            {
                new (newStorage + i) Mat4x4(newStorage[i]);
            }

            for (size_t i = 0; i < _Size; i++)
            {
                newStorage[i] = _Data._Mat4x4[i];
            }

            ::operator delete(_Data._Mat4x4);
            _Data._Mat4x4 = newStorage;
            break;
        }
        default:
            break;
        }
    }

    void Datum::Pop()
    {
        FIEA_ERROR(!_IsExternal);
        FIEA_ERROR(_Type != _DatumType::Unknown && _Size > 0);

        if (_Type == _DatumType::String)
        {
            _Data._String[_Size - 1].~basic_string();
        }
        --_Size;
    }

    void Datum::Clear()
    {
        if (_IsExternal) return;
        _Size = 0;
    }

    size_t Datum::Size() const
    {
        return _Size;
    }

    size_t Datum::Capacity() const
    {
        return _Capacity;
    }

    void Datum::SetFromString(const string& value, size_t idx)
    {
        FIEA_ERROR(idx < _Size);

        switch (_Type)
        {
        case _DatumType::Int:
            _Data._Int[idx] = std::stoi(value.c_str());
            break;
        case _DatumType::Float:
            _Data._Float[idx] = std::stof(value.c_str());
            break;
        case _DatumType::String:
            _Data._String[idx] = value;
            break;
        case _DatumType::Vec4:
        {
            glm::f32vec4 vec;
            int result = sscanf(value.c_str(), "vec4(%f, %f, %f, %f)", &vec.x, &vec.y, &vec.z, &vec.w);
            FIEA_ERROR(result == 4);
            _Data._Vec4[idx] = vec;
            break;
        }
        case _DatumType::Mat4x4:
        {
            glm::f32mat4x4 mat;
            int result = sscanf(value.c_str(),
                "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
                &mat[0][0], &mat[0][1], &mat[0][2], &mat[0][3],
                &mat[1][0], &mat[1][1], &mat[1][2], &mat[1][3],
                &mat[2][0], &mat[2][1], &mat[2][2], &mat[2][3],
                &mat[3][0], &mat[3][1], &mat[3][2], &mat[3][3]);
            FIEA_ERROR(result == 16);
            _Data._Mat4x4[idx] = mat;
            break;
        }
        default:
            FIEA_ERROR(false);
        }
    }

    string Datum::GetToString(size_t idx) const
    {
        FIEA_ERROR(idx < _Size);

        switch (_Type)
        {
        case _DatumType::Int:
            return std::to_string(_Data._Int[idx]).c_str();
        case _DatumType::Float:
            return std::to_string(_Data._Float[idx]).c_str();
        case _DatumType::String:
            return _Data._String[idx];
        case _DatumType::Vec4:
            return glm::to_string(_Data._Vec4[idx]).c_str();
        case _DatumType::Mat4x4:
            return glm::to_string(_Data._Mat4x4[idx]).c_str();
        default:
            FIEA_ERROR(false);
            return "";
        }
    }

    void Datum::PushFromString(const string& value)
    {
        switch (_Type)
        {
        case _DatumType::Int:
            Push(std::stoi(value.c_str()));
            break;
        case _DatumType::Float:
            Push(std::stof(value.c_str()));
            break;
        case _DatumType::String:
            Push(value);
            break;
        case _DatumType::Vec4:
        {
            glm::f32vec4 vec;
            int count = sscanf(
                value.c_str(),
                "vec4(%f, %f, %f, %f)",
                &vec.x, &vec.y, &vec.z, &vec.w
            );
            FIEA_ERROR(count == 4);
            Push(vec);
            break;
        }
        case _DatumType::Mat4x4:
        {
            glm::f32mat4x4 mat;
            int count = sscanf(
                value.c_str(),
                "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
                &mat[0][0], &mat[0][1], &mat[0][2], &mat[0][3],
                &mat[1][0], &mat[1][1], &mat[1][2], &mat[1][3],
                &mat[2][0], &mat[2][1], &mat[2][2], &mat[2][3],
                &mat[3][0], &mat[3][1], &mat[3][2], &mat[3][3]
            );
            FIEA_ERROR(count == 16);
            Push(mat);
            break;
        }
        default:
            FIEA_ERROR(false);
        }
    }

    void Datum::RemoveAt(size_t idx)
    {
        FIEA_ERROR(idx < _Size);

        if (_Type == _DatumType::Table)
        {
            for (size_t i = idx; i < _Size - 1; ++i)
            {
                _Data._Table[i] = _Data._Table[i + 1];
            }

            _Data._Table[_Size - 1] = nullptr;
            --_Size;
        }
    }
}