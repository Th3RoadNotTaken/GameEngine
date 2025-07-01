#include "Content/Datum.h"

namespace Fiea::Engine::Content
{
    template<typename U>
    void Datum::Set(U&& val, size_t idx)
    {
        FIEA_ERROR(_Size != 0 && idx < _Size);

        if constexpr (std::is_same_v<U, int32_t>)
        {
            FIEA_ERROR(_Type == _DatumType::Int);
            _Data._Int[idx] = std::forward<U>(val);
        }
        else if constexpr (std::is_same_v<U, float>)
        {
            FIEA_ERROR(_Type == _DatumType::Float);
            _Data._Float[idx] = std::forward<U>(val);
        }
        else if constexpr (std::is_same_v<U, string>)
        {
            FIEA_ERROR(_Type == _DatumType::String);
            _Data._String[idx] = std::forward<U>(val);
        }
        else if constexpr (std::is_convertible_v<std::remove_cv_t<std::remove_reference_t<U>>, Scope*>)
        {
            FIEA_ERROR(_Type == _DatumType::Table);
            _Data._Table[idx] = std::forward<U>(val);
        }
        else if constexpr (std::is_convertible_v<std::remove_cv_t<std::remove_reference_t<U>>, RTTI*>)
        {
            FIEA_ERROR(_Type == _DatumType::Pointer);
            _Data._Pointer[idx] = std::forward<U>(val);
        }
        else if constexpr (std::is_same_v<U, Vec4>)
        {
            FIEA_ERROR(_Type == _DatumType::Vec4);
            _Data._Vec4[idx] = std::forward<U>(val);
        }
        else if constexpr (std::is_same_v<U, Mat4x4>)
        {
            FIEA_ERROR(_Type == _DatumType::Mat4x4);
            _Data._Mat4x4[idx] = std::forward<U>(val);
        }
        else
        {
            FIEA_ERROR(_Type != _DatumType::Unknown);
        }
    }

    template<typename U>
    inline void Datum::Push(U&& val)
    {
        FIEA_ERROR(!_IsExternal);

        if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, int32_t>)
        {
            if (_Type == _DatumType::Unknown)
            {
                _Data._Int = static_cast<int32_t*>(::operator new(sizeof(int32_t)));
                new (_Data._Int) int32_t(std::forward<U>(val));

                _Data._Int[0] = std::forward<U>(val);
                _Size = 1;
                _Capacity = 1;
                _Type = _DatumType::Int;

                return;
            }

            FIEA_ERROR(_Type == _DatumType::Int);

            if (_Size == _Capacity)
            {
                ResizeArray(_DatumType::Int);
                _Data._Int[_Size++] = std::forward<U>(val);
            }
            else
            {
                _Data._Int[_Size++] = std::forward<U>(val);
            }
        }
        else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, float>)
        {
            if (_Type == _DatumType::Unknown)
            {
                _Data._Float = static_cast<float*>(::operator new(sizeof(float)));
                new (_Data._Float) float(std::forward<U>(val));

                _Data._Float[0] = std::forward<U>(val);
                _Size = 1;
                _Capacity = 1;
                _Type = _DatumType::Float;

                return;
            }

            FIEA_ERROR(_Type == _DatumType::Float);

            if (_Size == _Capacity)
            {
                ResizeArray(_DatumType::Float);
                _Data._Float[_Size++] = std::forward<U>(val);
            }
            else
            {
                _Data._Float[_Size++] = std::forward<U>(val);
            }
        }
        else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, string>)
        {
            if (_Type == _DatumType::Unknown)
            {
                _Data._String = static_cast<string*>(::operator new(sizeof(string)));
                new (_Data._String) string(std::forward<U>(val));

                _Size = 1;
                _Capacity = 1;
                _Type = _DatumType::String;

                return;
            }

            FIEA_ERROR(_Type == _DatumType::String);

            if (_Size == _Capacity)
            {
                ResizeArray(_DatumType::String);
                _Data._String[_Size++] = std::forward<U>(val);
            }
            else
            {
                _Data._String[_Size++] = std::forward<U>(val);
            }
        }
        else if constexpr (std::is_convertible_v<std::remove_cv_t<std::remove_reference_t<U>>, Scope*>)
        {
            if (_Type == _DatumType::Unknown)
            {
                _Data._Table = static_cast<Scope**>(::operator new(sizeof(Scope*)));
                new (_Data._Table) Scope* (std::forward<U>(val));

                _Size = 1;
                _Capacity = 1;
                _Type = _DatumType::Table;

                return;
            }

            FIEA_ERROR(_Type == _DatumType::Table);

            if (_Size == _Capacity)
            {
                ResizeArray(_DatumType::Table);
                _Data._Table[_Size++] = std::forward<U>(val);
            }
            else
            {
                _Data._Table[_Size++] = std::forward<U>(val);
            }
        }
        else if constexpr (std::is_convertible_v<std::remove_cv_t<std::remove_reference_t<U>>, RTTI*>)
        {
            if (_Type == _DatumType::Unknown)
            {
                _Data._Pointer = static_cast<RTTI**>(::operator new(sizeof(RTTI*)));
                new (_Data._Pointer) RTTI*(std::forward<U>(val));

                _Size = 1;
                _Capacity = 1;
                _Type = _DatumType::Pointer;

                return;
            }

            FIEA_ERROR(_Type == _DatumType::Pointer);

            if (_Size == _Capacity)
            {
                ResizeArray(_DatumType::Pointer);
                _Data._Pointer[_Size++] = std::forward<U>(val);
            }
            else
            {
                _Data._Pointer[_Size++] = std::forward<U>(val);
            }
        }
        else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, Vec4>)
        {
            if (_Type == _DatumType::Unknown)
            {
                _Data._Vec4 = static_cast<Vec4*>(::operator new(sizeof(Vec4)));
                new (_Data._Vec4) Vec4(std::forward<U>(val));

                _Data._Vec4[0] = std::forward<U>(val);
                _Size = 1;
                _Capacity = 1;
                _Type = _DatumType::Vec4;

                return;
            }

            FIEA_ERROR(_Type == _DatumType::Vec4);

            if (_Size == _Capacity)
            {
                ResizeArray(_DatumType::Vec4);
                _Data._Vec4[_Size++] = std::forward<U>(val);
            }
            else
            {
                _Data._Vec4[_Size++] = std::forward<U>(val);
            }
        }
        else if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<U>>, Mat4x4>)
        {
            if (_Type == _DatumType::Unknown)
            {
                _Data._Mat4x4 = static_cast<Mat4x4*>(::operator new(sizeof(Mat4x4)));
                new (_Data._Mat4x4) Mat4x4(std::forward<U>(val));

                _Data._Mat4x4[0] = std::forward<U>(val);
                _Size = 1;
                _Capacity = 1;
                _Type = _DatumType::Mat4x4;

                return;
            }

            FIEA_ERROR(_Type == _DatumType::Mat4x4);

            if (_Size == _Capacity)
            {
                ResizeArray(_DatumType::Mat4x4);
                _Data._Mat4x4[_Size++] = std::forward<U>(val);
            }
            else
            {
                _Data._Mat4x4[_Size++] = std::forward<U>(val);
            }
        }
        else
        {
            FIEA_ERROR(false);
        }
    }

    template<typename T>
    inline void Datum::SetStorage(T* items, size_t count)
    {
        _DataPointer = reinterpret_cast<void*>(items);
        _Size = count;
        _Capacity = count;
        _IsExternal = true;

        if constexpr (std::is_same_v<T, int32_t>)
        {
            FIEA_ERROR(_Type == _DatumType::Int || _Type == _DatumType::Unknown);
            _Type = _DatumType::Int;
            _Data._Int = items;
        }
        else if constexpr (std::is_same_v<T, float>)
        {
            FIEA_ERROR(_Type == _DatumType::Float || _Type == _DatumType::Unknown);
            _Type = _DatumType::Float;
            _Data._Float = items;
        }
        else if constexpr (std::is_same_v<T, string>)
        {
            FIEA_ERROR(_Type == _DatumType::String || _Type == _DatumType::Unknown);
            _Type = _DatumType::String;
            _Data._String = items;
        }
        else if constexpr (std::is_convertible_v<std::remove_cv_t<std::remove_reference_t<T>>, Scope*>)
        {
            FIEA_ERROR(_Type == _DatumType::Table || _Type == _DatumType::Unknown);
            _Type = _DatumType::Table;
            _Data._Table = items;
        }
        else if constexpr (std::is_convertible_v<T*, RTTI*>)
        {
            FIEA_ERROR(_Type == _DatumType::Pointer || _Type == _DatumType::Unknown);
            _Type = _DatumType::Pointer;
            _Data._Pointer = reinterpret_cast<RTTI**>(items);
        }
        else if constexpr (std::is_same_v<T, Vec4>)
        {
            FIEA_ERROR(_Type == _DatumType::Vec4 || _Type == _DatumType::Unknown);
            _Type = _DatumType::Vec4;
            _Data._Vec4 = items;
        }
        else if constexpr (std::is_same_v<T, Mat4x4>)
        {
            FIEA_ERROR(_Type == _DatumType::Mat4x4 || _Type == _DatumType::Unknown);
            _Type = _DatumType::Mat4x4;
            _Data._Mat4x4 = items;
        }
        else
        {
            FIEA_ERROR(false);
        }
    }
}