#pragma once
#include "FIEAGameEngine/Types.h"
#include "FIEAGameEngine/RTTI.h"
#include "FIEAGameEngine/ServiceMgr.h"
#include <iostream>
#include <unordered_map>

#define FACTORY_DEFINE(ProductType)                   \
RTTI_DEFINITIONS(ProductType##Factory);               

#define FACTORY_DEFINE_ABSTRACT(BaseType)                                    \
template<>                                                                   \
RTTI_DEFINITIONS(Fiea::Engine::Content::IAbstractFactory<BaseType>);                                                                  

#define FACTORY_DECLARE(ProductType, BaseType)                                      \
class ProductType##Factory : public Fiea::Engine::Content::IAbstractFactory<BaseType>                      \
{                                                                                   \
    RTTI_DECLARATIONS(ProductType##Factory, Fiea::Engine::Content::IAbstractFactory<BaseType>);             \
public:                                                                             \
    virtual BaseType* Create() override                                              \
    {                                                                               \
        return new ProductType();                                                    \
    }                                                                               \
    virtual std::string GetProductName() override                                    \
    {                                                                               \
        return #ProductType;                                                        \
    }                                                                               \
};

namespace Fiea::Engine::Content
{
    struct IFactoryBase : public RTTI
    {
        RTTI_DECLARATIONS(IFactoryBase, RTTI);
    public:
        virtual std::string GetProductName() = 0;
    };

    template <typename BaseType> struct IAbstractFactory : public IFactoryBase
    {
    public:
        RTTI_DECLARATIONS(IAbstractFactory, IFactoryBase);
    public:
        [[nodiscard]] virtual BaseType* Create() = 0;
    };

    struct IFactoryService
    {
        FIEA_SERVICE_INTERFACE;
    public:
        template<typename BaseType> void Register(std::unique_ptr<IAbstractFactory<BaseType>>&& concreteFactory)
        {
            FIEA_ASSERT(concreteFactory != nullptr);

            RegisterFactory(BaseType::TypeIdClass(), std::move(concreteFactory));
        }
        template <typename BaseType> BaseType* Create(const std::string& productName)
        {
            IFactoryBase* factory = GetFactory(BaseType::TypeIdClass(), productName);
            FIEA_ASSERT(factory != nullptr);

            auto abstractFactory = factory->As<IAbstractFactory<BaseType>>();
            FIEA_ASSERT(abstractFactory != nullptr);

            return abstractFactory->Create();
        }
    
    protected:
        virtual void RegisterFactory(RTTI::IdType baseId, std::unique_ptr<IFactoryBase>&& factory) = 0;
        virtual IFactoryBase* GetFactory(RTTI::IdType baseId, const std::string& productName) = 0;
    };

    class FactoryService : public IFactoryService
    {
        FIEA_SERVICE_BIND1(FactoryService, IFactoryService);

    protected:
        virtual void RegisterFactory(RTTI::IdType baseId, std::unique_ptr<IFactoryBase>&& factory) override;
        virtual IFactoryBase* GetFactory(RTTI::IdType baseId, const std::string& productName) override;

    private:
        std::unordered_map<RTTI::IdType, std::unordered_map<std::string, std::unique_ptr<IFactoryBase>>> _FactoryStorage;
    };
}