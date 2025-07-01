#pragma once
#include "FIEAGameEngine/Types.h"
#include "glm/glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

using Vec4 = glm::f32vec4;
using Mat4x4 = glm::f32mat4x4;

namespace Fiea::Engine
{
    class RTTI;
}

namespace Fiea::Engine::Content
{
    class Datum
    {
    public:
        enum class _DatumType {
            Unknown,
            Int,
            Float,
            String,
            Pointer,
            Table,
            Vec4,
            Mat4x4
        };
    private:
        friend class Scope;

        union _DataUnion
        {
            int32_t* _Int;
            float* _Float;
            string* _String;
            RTTI** _Pointer;
            Scope** _Table;
            Vec4* _Vec4;
            Mat4x4* _Mat4x4;
        };

        _DataUnion _Data;
        _DatumType _Type;
        size_t _Size;
        size_t _Capacity;

        void* _DataPointer;
        bool _IsExternal;

    public:
        Datum();
        ~Datum();
        Datum(const Datum& other);
        Datum& operator=(const Datum& rhs);
        Datum(Datum&& other) noexcept;
        Datum& operator=(Datum&& rhs);

        // Type specific constructors
        explicit Datum(int32_t value);
        explicit Datum(float value);
        explicit Datum(string value);
        explicit Datum(RTTI* value);
        explicit Datum(Scope* value);
        explicit Datum(Vec4 value);
        explicit Datum(Mat4x4 value);

        // Initializer list constructors
        Datum(std::initializer_list<int32_t> values);
        Datum(std::initializer_list<float> values);
        Datum(std::initializer_list<string> values);
        Datum(std::initializer_list<RTTI*> values);
        Datum(std::initializer_list<Scope*> values);
        Datum(std::initializer_list<Vec4> values);
        Datum(std::initializer_list<Mat4x4> values);

        // Specialized assignment operator overloads
        Datum& operator=(const int32_t value);
        Datum& operator=(const float value);
        Datum& operator=(const string value);
        Datum& operator=(RTTI* const value);
        Datum& operator=(Scope* const value);
        Datum& operator=(const Vec4 value);
        Datum& operator=(const Mat4x4 value);

        bool operator==(const Datum& other) const;
        bool operator!=(const Datum& other) const;

        void RemoveAt(size_t idx);

        void SetType(_DatumType typeToSet);
        bool IsType(_DatumType typeToCheck) const;

        template<typename U>
        void Set(U&& val, size_t idx = 0);

        int32_t& GetAsInt(size_t idx = 0);
        float& GetAsFloat(size_t idx = 0);
        string& GetAsString(size_t idx = 0);
        RTTI*& GetAsPointer(size_t idx = 0);
        Scope*& GetAsTable(size_t idx = 0);
        Vec4& GetAsVec4(size_t idx = 0);
        Mat4x4& GetAsMat4x4(size_t idx = 0);

        const int32_t& GetAsInt(size_t idx = 0) const;
        const float& GetAsFloat(size_t idx = 0) const;
        const string& GetAsString(size_t idx = 0) const;
        RTTI* const& GetAsPointer(size_t idx = 0) const;
        Scope* const& GetAsTable(size_t idx = 0) const;
        const Vec4& GetAsVec4(size_t idx = 0) const;
        const Mat4x4& GetAsMat4x4(size_t idx = 0) const;

        template<typename U>
        void Push(U&& val);

        void ResizeArray(const _DatumType& type);
        void Pop();
        void Clear();

        size_t Size() const;
        size_t Capacity() const;

        void SetFromString(const string&, size_t idx = 0);
        string GetToString(size_t idx = 0) const;
        void PushFromString(const string&);

        template<typename T>
        void SetStorage(T* items, size_t count = 1);
    };
}

#include "Content/Datum.inl"