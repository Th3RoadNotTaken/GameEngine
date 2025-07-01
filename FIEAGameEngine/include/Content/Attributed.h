#pragma once
#include "FieaGameEngine/Types.h"
#include "Content/Datum.h"
#include "Content/Scope.h"
#include "FIEAGameEngine/RTTI.h"
#include <vector>
#include <functional>

namespace Fiea::Engine::Content
{
    struct Signature
    {
        const char* _Name;
        size_t _Offset;
        Datum::_DatumType _Type;
        size_t _Count;
    };

    using ClassDefinition = std::vector<Signature>;

    class Attributed : public Scope
    {
        RTTI_DECLARATIONS(Attributed, Scope);
    public:
        Attributed() = delete;
        explicit Attributed(std::function<ClassDefinition()> defGenerator, const RTTI::IdType& idType);

        Attributed(const Attributed& other);
        Attributed(Attributed&& other) noexcept;
        Attributed& operator=(const Attributed& rhs);
        Attributed& operator=(Attributed&& rhs) noexcept;
        virtual ~Attributed() = default;

        [[nodiscard]] Attributed* Clone() const override;

        // Utility methods
        bool IsAttribute(const string& key);
        bool IsClassAttribute(const string& key);
        bool IsInstanceAttribute(const string& key);
        Datum& AppendInstanceAttribute(const string& key);
        std::vector<string> Attributes();
        std::vector<string> ClassAttributes();
        std::vector<string> InstanceAttributes();

    private:
        void Target(RTTI::IdType idType);
        void Retarget(const Attributed& rhs);
    };
}