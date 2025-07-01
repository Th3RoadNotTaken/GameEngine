#pragma once
#include "FIEAGameEngine/Types.h"
#include "FIEAGameEngine/RTTI.h"
#include "Content/Datum.h"
#include "FIEAGameEngine/FactoryService.h"
#include <vector>
#include <unordered_map>

namespace Fiea::Engine::Content
{
    class Scope : public Fiea::Engine::RTTI
    {
        RTTI_DECLARATIONS(Scope, RTTI);

    private:
        std::vector<const string*> _OrderedDatums;
        std::unordered_map<string, Datum> _DatumLookup;

        Scope* _Parent = nullptr;

        void Copy(const Scope& rhs);
        void Move(Scope&& rhs);

    protected:
        std::vector<const string*> GetOrderedDatums() { return _OrderedDatums; }

    public:
        Scope() = default;
        Scope(const Scope& rhs);
        Scope(Scope&& rhs) noexcept;
        Scope& operator=(const Scope& rhs);
        Scope& operator=(Scope&& rhs) noexcept;
        virtual ~Scope();

        bool operator==(const Scope& rhs) const;
        bool operator!=(const Scope& rhs) const;

        Datum* Find(const string& key);
        const Datum* Find(const string& key) const;
        Datum& Append(const string& key);
        void RemoveDatum(const string& key);

        Datum* Search(const string& key);

        Datum& operator[](const string& key);

        Datum& operator[](size_t idx);

        size_t Size() const;
        void Clear();

        Scope& AppendScope(const string& key, Scope* scope = nullptr);
        void Adopt(const string& key, Scope& child);

        Scope* GetParent() const;
        void SetParent(Scope& parent);

        [[nodiscard]] Scope* Orphan();

        Datum* FindContainedScope(const Scope& child, size_t& idx);
        const Datum* FindContainedScope(const Scope& child, size_t& idx) const;

        bool IsAncestorOf(const Scope& descendent) const;
        bool IsDescendentOf(const Scope& ancestor) const;

        virtual Scope* Clone() const;
    };

    FACTORY_DECLARE(Scope, Scope);
}