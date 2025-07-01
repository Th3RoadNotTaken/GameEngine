#include "Content/Scope.h"

namespace Fiea::Engine::Content
{
    RTTI_DEFINITIONS(Scope);

    /**
     * @brief Copy constructor.
     * @param rhs The Scope to copy from.
     */
    Scope::Scope(const Scope& rhs)
    {
        Copy(rhs);
    }

    /**
     * @brief Move constructor.
     * @param rhs The Scope to move from.
     */
    Scope::Scope(Scope&& rhs) noexcept
    {
        Move(std::move(rhs));
    }

    /**
     * @brief Copy assignment operator.
     * @param rhs The Scope to copy from.
     * @return Reference to this Scope.
     */
    Scope& Scope::operator=(const Scope& rhs)
    {
        if (this != &rhs)
        {
            Clear();
            Copy(rhs);
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param rhs The Scope to move from.
     * @return Reference to this Scope.
     */
    Scope& Scope::operator=(Scope&& rhs) noexcept
    {
        if (this != &rhs)
        {
            Clear();
            Move(std::move(rhs));
        }
        return *this;
    }

    /**
     * @brief Destructor. Clears the Scope.
     */
    Scope::~Scope()
    {
        Clear();
    }

    /**
     * @brief Copies the contents of another Scope.
     * @param rhs The Scope to copy from.
     */
    void Scope::Copy(const Scope& rhs)
    {
        Clear();

        for (const string* keyPtr : rhs._OrderedDatums)
        {
            const string& key = *keyPtr;
            auto srcItr = rhs._DatumLookup.find(key);
            if (srcItr == rhs._DatumLookup.end())
            {
                continue;
            }
            const Datum& srcDatum = srcItr->second;
            Datum newDatum;

            if (srcDatum.IsType(Datum::_DatumType::Table))
            {
                newDatum.SetType(Datum::_DatumType::Table);
                if (srcDatum.Size() == 0)
                    continue;
                for (size_t i = 0; i < srcDatum.Size(); ++i)
                {
                    Scope* child = srcDatum.GetAsTable(i);
                    if (child != nullptr)
                    {
                        Scope* newChild = child->Clone();
                        newChild->_Parent = this;
                        newDatum.Push(newChild);
                    }
                }
            }
            else
            {
                newDatum = srcDatum;
            }

            auto result = _DatumLookup.try_emplace(key, newDatum);
            _OrderedDatums.push_back(&(result.first->first));
        }
    }

    /**
     * @brief Moves the contents of another Scope.
     * @param rhs The Scope to move from.
     */
    void Scope::Move(Scope&& rhs)
    {
        _Parent = rhs._Parent;
        _DatumLookup = std::move(rhs._DatumLookup);
        _OrderedDatums = std::move(rhs._OrderedDatums);

        for (auto& [key, datum] : _DatumLookup)
        {
            if (datum.IsType(Datum::_DatumType::Table))
            {
                for (size_t i = 0; i < datum.Size(); ++i)
                {
                    Scope* child = datum.GetAsTable(i);
                    if (child != nullptr)
                    {
                        child->_Parent = this;
                    }
                }
            }
        }

        rhs._Parent = nullptr;
        rhs._DatumLookup.clear();
        rhs._OrderedDatums.clear();
    }

    /**
     * @brief Equality operator.
     * @param rhs The Scope to compare with.
     * @return True if the scopes are equal, false otherwise.
     */
    bool Scope::operator==(const Scope& rhs) const
    {
        if (_DatumLookup.size() != rhs._DatumLookup.size())
            return false;

        for (size_t i = 0; i < _OrderedDatums.size(); ++i)
        {
            if (*_OrderedDatums[i] != *rhs._OrderedDatums[i])
                return false;
        }

        for (const auto& key : _OrderedDatums)
        {
            auto itr1 = _DatumLookup.find(*key);
            auto itr2 = rhs._DatumLookup.find(*key);

            if (itr1 == _DatumLookup.end() || itr2 == rhs._DatumLookup.end())
                return false;
            if (itr1->second != itr2->second)
                return false;
        }

        return true;
    }

    /**
     * @brief Inequality operator.
     * @param rhs The Scope to compare with.
     * @return True if the scopes are not equal, false otherwise.
     */
    bool Scope::operator!=(const Scope& rhs) const
    {
        if (_DatumLookup.size() != rhs._DatumLookup.size())
            return true;

        for (size_t i = 0; i < _OrderedDatums.size(); ++i)
        {
            if (*_OrderedDatums[i] != *rhs._OrderedDatums[i])
                return true;
        }

        for (const auto& key : _OrderedDatums)
        {
            auto itr1 = _DatumLookup.find(*key);
            auto itr2 = rhs._DatumLookup.find(*key);

            if (itr1 == _DatumLookup.end() || itr2 == rhs._DatumLookup.end())
                return true;
            if (itr1->second != itr2->second)
                return true;
        }

        return false;
    }

    /**
    * @brief Finds a Datum by key.
    * @param key The key to search for.
    * @return A pointer to the Datum if found, otherwise nullptr.
    */
    Datum* Scope::Find(const string& key)
    {
        auto it = _DatumLookup.find(key);
        return it != _DatumLookup.end() ? &it->second : nullptr;
    }

    /**
    * @brief Finds a Datum by key (const version).
    * @param key The key to search for.
    * @return A pointer to the Datum if found, otherwise nullptr.
    */
    const Datum* Scope::Find(const string& key) const
    {
        auto it = _DatumLookup.find(key);
        return it != _DatumLookup.end() ? &it->second : nullptr;
    }

    /**
     * @brief Appends a new Datum with the given key.
     * @param key The key for the new Datum.
     * @return Reference to the newly created or existing Datum.
     */
    Datum& Scope::Append(const string& key)
    {
        auto [it, inserted] = _DatumLookup.try_emplace(key, Datum());

        if (inserted)
        {
            _OrderedDatums.push_back(&(it->first));
        }

        return it->second;
    }

    void Scope::RemoveDatum(const string& key)
    {
        auto it = _DatumLookup.find(key);
        if (it == _DatumLookup.end())
            return;

        size_t idx = -1;
        for (size_t i = 0; i < _OrderedDatums.size(); ++i)
        {
            if (key == *_OrderedDatums[i])
            {
                idx = i;
                break;
            }
        }
        if (idx == -1) return;

        _OrderedDatums.erase(_OrderedDatums.begin() + idx);
        _DatumLookup.erase(key);
    }

    Datum* Scope::Search(const string& key)
    {
        Scope* s = GetParent();
        if (s == nullptr)return nullptr;

        while (s != nullptr)
        {
            Datum* d = s->Find(key);
            if (d != nullptr)
                return d;
            else
                s = s->GetParent();
        }
        return nullptr;
    }

    /**
     * @brief Overloads operator[] for accessing a Datum by key.
     * If the key does not exist in the Scope, it is appended.
     * @param key The key associated with the Datum.
     * @return Datum& A reference to the Datum corresponding to the key.
     */
    Datum& Scope::operator[](const string& key)
    {
        return Append(key);
    }

    /**
     * @brief Overloads operator[] for accessing a Datum by index.
     * Retrieves the Datum corresponding to the given index in the ordered list.
     * @param idx The index of the Datum.
     * @return Datum& A reference to the Datum at the specified index.
     */
    Datum& Scope::operator[](size_t idx)
    {
        FIEA_ERROR(idx < _OrderedDatums.size());

        auto it = _DatumLookup.find(*_OrderedDatums[idx]);
        FIEA_ERROR(it != _DatumLookup.end());

        return it->second;
    }

    /**
     * @brief Returns the number of key-value pairs in the Scope.
     * @return size_t The number of entries in the Scope.
     */
    size_t Scope::Size() const
    {
        return _DatumLookup.size();
    }

    /**
     * @brief Clears all data in the Scope.
     */
    void Scope::Clear()
    {
        for (auto& [key, datum] : _DatumLookup)
        {
            if (datum.IsType(Datum::_DatumType::Table))
            {
                for (size_t i = 0; i < datum.Size(); ++i)
                {
                    Scope* child = datum.GetAsTable(i);
                    if (child != nullptr && child->GetParent() == this)
                    {
                        child->_Parent = nullptr;
                        delete child;
                    }
                }
            }
        }
        _OrderedDatums.clear();
        _DatumLookup.clear();
    }

    /**
     * @brief Appends a new Scope as a child under the specified key.
     * @param key The key under which the child Scope is to be appended.
     * @param scope A pointer to an optional pre-created Scope. If nullptr, a new Scope is allocated.
     * @return Scope& A reference to the appended child Scope.
     */
    Scope& Scope::AppendScope(const string& key, Scope* scope)
    {
        Datum* d = Find(key);

        if (d == nullptr)
        {
            d = &Append(key);
            d->SetType(Datum::_DatumType::Table);
        }
        else if(!d->IsType(Datum::_DatumType::Table))
        {
            FIEA_ERROR(d->IsType(Datum::_DatumType::Table));
        }

        Scope* child = (scope == nullptr) ? new Scope() : scope;
        child->SetParent(*this);

        d->Push(child);
        return *child;
    }

    /**
     * @brief Adopts an existing Scope as a child under the specified key.
     * @param key The key under which to adopt the child Scope.
     * @param child A reference to the child Scope to be adopted.
     */
    void Scope::Adopt(const string& key, Scope& child)
    {
        if (child._Parent != nullptr)
        {
            Scope* orphanedChild = child.Orphan();
        }

        Datum* d = Find(key);

        if (d == nullptr)
        {
            d = &Append(key);
            d->SetType(Datum::_DatumType::Table);
        }
        else if (!d->IsType(Datum::_DatumType::Table))
        {
            FIEA_ERROR(d->IsType(Datum::_DatumType::Table));
        }

        child._Parent = this;
        d->Push(&child);
    }

    Scope* Scope::GetParent() const
    {
        return _Parent;
    }

    void Scope::SetParent(Scope& parent)
    {
        _Parent = &parent;
    }

    /**
     * @brief Removes this Scope from its parent.
     * @return Scope* A pointer to this Scope after it has been orphaned.
     */
    Scope* Scope::Orphan()
    {
        if (_Parent == nullptr)
            return this;

        size_t index;
        Datum* parentDatum = _Parent->FindContainedScope(*this, index);

        if (parentDatum != nullptr)
        {
            parentDatum->RemoveAt(index);
        }

        _Parent = nullptr;
        return this;
    }

    /**
     * @brief Finds the Datum that contains the specified child Scope.
     * @param child A reference to the child Scope to search for.
     * @param idx [out] The index within the Datum where the child was found. Set to -1 if not found.
     * @return Datum* A pointer to the Datum containing the child, or nullptr if not found.
     */
    Datum* Scope::FindContainedScope(const Scope& child, size_t& idx)
    {
        idx = 0;
        for (auto itr = _OrderedDatums.begin(); itr != _OrderedDatums.end(); ++itr, ++idx)
        {
            auto it = _DatumLookup.find(**itr);

            if (it != _DatumLookup.end() && it->second.IsType(Datum::_DatumType::Table))
            {
                if (it->second.Size() > 0)
                {
                    // Iterate over all elements in this Datum.
                    for (size_t j = 0; j < it->second.Size(); ++j)
                    {
                        if (it->second.GetAsTable(j) == &child)
                        {
                            idx = j;
                            return &it->second;
                        }
                    }
                }
            }
        }
        idx = -1;
        return nullptr;
    }

    /**
     * @brief Finds the Datum that contains the specified child Scope (const version).
     * @param child A reference to the child Scope to search for.
     * @param idx [out] The index within the Datum where the child was found. Set to -1 if not found.
     * @return Datum* A pointer to the Datum containing the child, or nullptr if not found.
     */
    const Datum* Scope::FindContainedScope(const Scope& child, size_t& idx) const
    {
        idx = 0;
        for (auto itr = _OrderedDatums.begin(); itr != _OrderedDatums.end(); ++itr, ++idx)
        {
            auto it = _DatumLookup.find(**itr);

            if (it != _DatumLookup.end() && it->second.IsType(Datum::_DatumType::Table))
            {
                if (&child == it->second.GetAsTable())
                {
                    return &it->second;
                }
            }
        }
        idx = -1;
        return nullptr;
    }

    /**
     * @brief Checks if this Scope is an ancestor of the specified descendent Scope.
     * @param descendent The Scope to check.
     * @return true if this Scope is an ancestor of the descendent, false otherwise.
     */
    bool Scope::IsAncestorOf(const Scope& descendent) const
    {
        const Scope* parentScope = descendent.GetParent();

        if (parentScope == nullptr)
            return false;

        while (parentScope != nullptr)
        {
            if (parentScope == this)
                return true;
            parentScope = parentScope->GetParent();
        }
        return false;
    }

    /**
     * @brief Checks if this Scope is a descendent of the specified ancestor Scope.
     * @param ancestor The potential ancestor Scope.
     * @return true if this Scope is a descendent of the ancestor, false otherwise.
     */
    bool Scope::IsDescendentOf(const Scope& ancestor) const
    {
        return ancestor.IsAncestorOf(*this);
    }

    Scope* Scope::Clone() const
    {
        return new Scope(*this);
    }

    FACTORY_DEFINE_ABSTRACT(Fiea::Engine::Content::Scope);
    FACTORY_DEFINE(Scope);
}