#include "Content/Attributed.h"
#include "FieaGameEngine/ContentService.h"

namespace Fiea::Engine::Content
{
    RTTI_DEFINITIONS(Attributed);
    /**
     * @brief Copy constructor for Attributed.
     * @param other The Attributed object to copy from.
     */
    Attributed::Attributed(const Attributed& other) : Scope(other)
    {
        Retarget(other);
    }

    /**
     * @brief Move constructor for Attributed.
     * @param other The Attributed object to move from.
     */
    Attributed::Attributed(Attributed&& other) noexcept : Scope(std::move(other))
    {
        Retarget(other);
    }

    /**
     * @brief Copy assignment operator.
     * @param rhs The Attributed object to copy from.
     * @return Reference to this Attributed object.
     */
    Attributed& Attributed::operator=(const Attributed& rhs)
    {
        if (this != &rhs)
        {
            Scope::operator=(rhs);
            Retarget(rhs);
        }
        return *this;
    }

    /**
     * @brief Move assignment operator.
     * @param rhs The Attributed object to move from.
     * @return Reference to this Attributed object.
     */
    Attributed& Attributed::operator=(Attributed&& rhs) noexcept
    {
        if (this != &rhs)
        {
            Scope::operator=(std::move(rhs));
            Retarget(rhs);
        }
        return *this;
    }

    /**
     * @brief Creates a deep copy of this Attributed object.
     * @return A pointer to the cloned object.
     */
    Attributed* Attributed::Clone() const
    {
        return new Attributed(*this);
    }

    /**
     * @brief Binds the class attributes to memory offsets using the provided RTTI Id.
     * @param idType The RTTI IdType used to fetch class definitions.
     */
    void Attributed::Target(RTTI::IdType idType)
    {
        ITypeProvider* typeProvider = ServiceManager::ProvideInterface<ITypeProvider>();
        ClassDefinition _Def;

        if (typeProvider != nullptr)
        {
            const ClassDefinition* retPtr = typeProvider->GetClassDefinition(idType);
            if (retPtr == nullptr)return;

            _Def = *retPtr;
        }
        for (size_t i = 0; i < _Def.size(); ++i)
        {
            Datum& d = this->Append(_Def[i]._Name);

            if (_Def[i]._Offset == 0)
            {
                d.SetType(_Def[i]._Type);   
                d.ResizeArray(_Def[i]._Type);
                continue;
            }

            switch (_Def[i]._Type)
            {
            case Datum::_DatumType::Int:
            {
                int32_t* intPtr = reinterpret_cast<int32_t*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                d.SetStorage(intPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Float:
            {
                float* floatPtr = reinterpret_cast<float*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                d.SetStorage(floatPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::String:
            {
                string* strPtr = reinterpret_cast<string*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                d.SetStorage(strPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Pointer:
            {
                RTTI* rttiPtr = reinterpret_cast<RTTI*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                d.SetStorage(rttiPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Table:
            {
                Scope** scopePtr = reinterpret_cast<Scope**>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                d.SetStorage(scopePtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Vec4:
            {
                Vec4* vecPtr = reinterpret_cast<Vec4*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                d.SetStorage(vecPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Mat4x4:
            {
                Mat4x4* matPtr = reinterpret_cast<Mat4x4*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                d.SetStorage(matPtr, _Def[i]._Count);
                break;
            }
            default:
            {
                FIEA_ERROR(_Def[i]._Type == Datum::_DatumType::Unknown);
                break;
            }
            }
        }
    }

    /**
     * @brief Retargets attribute bindings after copying from another Attributed object.
     * @param rhs The source Attributed object.
     */
    void Attributed::Retarget(const Attributed& rhs)
    {
        ITypeProvider* typeProvider = ServiceManager::ProvideInterface<ITypeProvider>();
        ClassDefinition _Def;

        if (typeProvider != nullptr)
        {
            const ClassDefinition* retPtr = typeProvider->GetClassDefinition(rhs.TypeIdInstance());
            if (retPtr == nullptr)return;

            _Def = *retPtr;
        }
        for (size_t i = 0; i < _Def.size(); ++i)
        {
            Datum* d = this->Find(_Def[i]._Name);
            if (_Def[i]._Offset == 0)
            {
                if (d != nullptr)
                {
                    d->SetType(_Def[i]._Type);
                    d->ResizeArray(_Def[i]._Type);
                }
                continue;
            }

            switch (_Def[i]._Type)
            {
            case Datum::_DatumType::Int:
            {
                int32_t* intPtr = reinterpret_cast<int32_t*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                if(d!=nullptr)
                    d->SetStorage(intPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Float:
            {
                float* floatPtr = reinterpret_cast<float*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                if(d!=nullptr)
                    d->SetStorage(floatPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::String:
            {
                string* strPtr = reinterpret_cast<string*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                if(d!=nullptr)
                    d->SetStorage(strPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Pointer:
            {
                RTTI* rttiPtr = reinterpret_cast<RTTI*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                if(d!=nullptr)
                    d->SetStorage(rttiPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Table:
            {
                Scope* scopePtr = reinterpret_cast<Scope*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                if(d!=nullptr)
                    d->SetStorage(&scopePtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Vec4:
            {
                Vec4* vecPtr = reinterpret_cast<Vec4*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                if (d != nullptr)
                    d->SetStorage(vecPtr, _Def[i]._Count);
                break;
            }
            case Datum::_DatumType::Mat4x4:
            {
                Mat4x4* matPtr = reinterpret_cast<Mat4x4*>(reinterpret_cast<char*>(static_cast<void*>(this)) + _Def[i]._Offset);
                if (d != nullptr)
                    d->SetStorage(matPtr, _Def[i]._Count);
                break;
            }
            default:
            {
                FIEA_ERROR(_Def[i]._Type == Datum::_DatumType::Unknown);
                break;
            }
            }
        }
    }

    /**
     * @brief Constructor that initializes class attributes based on the provided class definition generator.
     * @param defGenerator A function that generates a class definition.
     * @param idType The RTTI IdType of the class.
     */
    Attributed::Attributed(std::function<ClassDefinition()> defGenerator, const RTTI::IdType& idType)
    {
        ITypeProvider* typeProvider = ServiceManager::ProvideInterface<ITypeProvider>();
        FIEA_ERROR(typeProvider != nullptr);

        if (typeProvider != nullptr)
        {
            if (typeProvider->GetClassDefinition(idType) == nullptr)
            {
                ITypeRegistrar* typeRegistrar = ServiceManager::ProvideInterface<ITypeRegistrar>();
                typeRegistrar->RegisterType(idType, defGenerator());
            }
            Target(idType);
        }
    }

    /**
     * @brief Checks if a given key is an attribute.
     * @param key The attribute key to check.
     * @return True if the key exists as an attribute, otherwise false.
     */
    bool Attributed::IsAttribute(const string& key)
    {
        return Find(key) == nullptr ? false : true;
    }

    /**
     * @brief Checks if a given key is a class attribute.
     * @param key The attribute key to check.
     * @return True if the key is a class attribute, otherwise false.
     */
    bool Attributed::IsClassAttribute(const string& key)
    {
        ITypeProvider* typeProvider = ServiceManager::ProvideInterface<ITypeProvider>();
        FIEA_ERROR(typeProvider != nullptr);

        if (typeProvider != nullptr)
        {
            const ClassDefinition* cd = typeProvider->GetClassDefinition(TypeIdInstance());

            if (cd == nullptr) return false;
            if (!IsAttribute(key)) return false;

            for (const auto& itr : *cd)
            {
                if (itr._Name == key)
                    return true;
            }
            return false;
        }
        else
            return false;
    }

    /**
     * @brief Checks if a given key is an instance attribute.
     * @param key The attribute key to check.
     * @return True if the key is an instance attribute, otherwise false.
     */
    bool Attributed::IsInstanceAttribute(const string& key)
    {
        return (IsAttribute(key) && !IsClassAttribute(key));
    }

    /**
     * @brief Appends a new instance attribute.
     * @param key The name of the new instance attribute.
     * @return Reference to the appended Datum object.
     */
    Datum& Attributed::AppendInstanceAttribute(const string& key)
    {
        FIEA_ERROR(!IsClassAttribute(key));
        
        return Append(key);
    }

    /**
     * @brief Retrieves all attribute names.
     * @return A vector of all attribute names.
     */
    std::vector<string> Attributed::Attributes()
    {
        std::vector<string> scopeNames;

        for (const auto& itr : Scope::GetOrderedDatums())
        {
            scopeNames.push_back(*itr);
        }

        return scopeNames;
    }

    /**
     * @brief Retrieves all class attribute names.
     * @return A vector of class attribute names.
     */
    std::vector<string> Attributed::ClassAttributes()
    {
        ITypeProvider* typeProvider = ServiceManager::ProvideInterface<ITypeProvider>();
        FIEA_ERROR(typeProvider != nullptr);

        std::vector<string> scopeNames;

        if (typeProvider != nullptr)
        {
            const ClassDefinition* defs = typeProvider->GetClassDefinition(TypeIdInstance());
            if (defs == nullptr)
                return scopeNames;

            for (const auto& itr : *defs)
            {
                scopeNames.push_back(itr._Name);
            }

            return scopeNames;
        }
        else
        {
            scopeNames.clear();
            return scopeNames;
        }
    }

    /**
     * @brief Retrieves all instance attribute names.
     * @return A vector of instance attribute names.
     */
    std::vector<string> Attributed::InstanceAttributes()
    {
        ITypeProvider* typeProvider = ServiceManager::ProvideInterface<ITypeProvider>();
        FIEA_ERROR(typeProvider != nullptr);

        std::vector<string> scopeNames;

        if (typeProvider != nullptr)
        {
            const ClassDefinition* defs = typeProvider->GetClassDefinition(TypeIdInstance());
            if (defs != nullptr)
            {
                for (const auto& itr : Scope::GetOrderedDatums())
                {
                    if (IsInstanceAttribute(*itr))
                    {
                        scopeNames.push_back(*itr);
                    }
                }
            }
            else
            {
                const ClassDefinition* defs = typeProvider->GetClassDefinition(TypeIdInstance());

                for (const auto& itr : *defs)
                {
                    scopeNames.push_back(itr._Name);
                }
            }
            return scopeNames;
        }
        else
        {
            scopeNames.clear();
            return scopeNames;
        }
    }
}