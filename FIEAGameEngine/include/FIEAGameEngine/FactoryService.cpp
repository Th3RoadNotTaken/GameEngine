#include "FIEAGameEngine/FactoryService.h"

namespace Fiea::Engine::Content
{
    RTTI_DEFINITIONS(IFactoryBase);

    void FactoryService::RegisterFactory(RTTI::IdType baseId, std::unique_ptr<IFactoryBase>&& factory)
    {
        FIEA_ASSERT(factory != nullptr);

        const std::string productName = factory->GetProductName();
        auto itr = _FactoryStorage.find(baseId);

        if (itr == _FactoryStorage.end())
        {
            _FactoryStorage[baseId][productName] = std::move(factory);
        }
        else
        {
            auto itr2 = itr->second.find(productName);
            if (itr2 == itr->second.end())
            {
                itr->second[productName] = std::move(factory);
            }
            else
            {
                // Already registered
                return;
            }
        }
    }

    IFactoryBase* FactoryService::GetFactory(RTTI::IdType baseId, const std::string& productName)
    {
        auto itr = _FactoryStorage.find(baseId);
        if (itr == _FactoryStorage.end())
            return nullptr;
        auto itr2 = itr->second.find(productName);
        if (itr2 == itr->second.end())
            return nullptr;
        else
            return itr2->second.get();
    }
}