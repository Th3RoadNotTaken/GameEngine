#include "FieaGameEngine/ContentService.h"
#include "../src/Content/Parser.h"

namespace Fiea::Engine::Content
{
    class ContentService : public ITypeRegistrar, public ITypeProvider, public IJsonConfig, public IJsonParser
    {
    private:
        FIEA_SERVICE_BIND4(ContentService, ITypeRegistrar, ITypeProvider, IJsonConfig, IJsonParser)

        std::unordered_map<Fiea::Engine::RTTI::IdType, ClassDefinition> _ContentMap;
        std::vector<std::unique_ptr<IParseHandler>> _ParseHandlers;

        Parser _Parser{ _ParseHandlers };

    public:
        virtual void RegisterType(Fiea::Engine::RTTI::IdType typeId, const ClassDefinition& def) override;
        virtual const ClassDefinition* GetClassDefinition(Fiea::Engine::RTTI::IdType typeId) const override;

        virtual void AddHandler(std::unique_ptr<IParseHandler> handler) override;
        virtual void RemoveHandler(const IParseHandler::InterfaceId& handlerType) override;

        virtual void Deserialize(const std::string& jsonString, ParseWriter& writer) override;
        virtual void Deserialize(std::istream& jsonStream, ParseWriter& writer) override;
        virtual void DeserializeFieldFromFile(const std::string& filePath, ParseWriter& writer) override;
    };

    void ContentService::RegisterType(Fiea::Engine::RTTI::IdType typeId, const ClassDefinition& def)
    {
        auto itr = _ContentMap.find(typeId);
        if (itr == _ContentMap.end())
        {
            _ContentMap[typeId] = def;
        }
    }

    const ClassDefinition* ContentService::GetClassDefinition(Fiea::Engine::RTTI::IdType typeId) const
    {
        auto itr = _ContentMap.find(typeId);

        if (itr == _ContentMap.end())
            return nullptr;
        else
            return &itr->second;
    }

    void ContentService::AddHandler(std::unique_ptr<IParseHandler> handler)
    {
        _ParseHandlers.push_back(std::move(handler));
    }

    void ContentService::RemoveHandler(const IParseHandler::InterfaceId& handlerType)
    {
        std::unique_ptr<IParseHandler> ptrToRemove;

        for (auto it = _ParseHandlers.begin(); it != _ParseHandlers.end(); )
        {
            if ((*it)->GetHandlerId() == handlerType)
            {
                it = _ParseHandlers.erase(it);
                break;
            }
            else
            {
                ++it;
            }
        }
    }

    void ContentService::Deserialize(const std::string& jsonString, ParseWriter& writer)
    {
        _Parser.Deserialize(jsonString, writer);
    }

    void ContentService::Deserialize(std::istream& jsonStream, ParseWriter& writer)
    {
        _Parser.Deserialize(jsonStream, writer);
    }

    void ContentService::DeserializeFieldFromFile(const std::string& filePath, ParseWriter& writer)
    {
        _Parser.DeserializeFieldFromFile(filePath, writer);
    }
}