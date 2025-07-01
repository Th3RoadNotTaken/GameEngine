#pragma once
#include "FIEAGameEngine/Types.h"
#include "FIEAGameEngine/ServiceMgr.h"
#include "FIEAGameEngine/RTTI.h"
#include "Content/Attributed.h"
#include "Content/Datum.h"
#include "Content/Scope.h"
#include <unordered_map>
#include "json/json.h"

namespace Fiea::Engine::Content
{
    #define FIEA_HANDLER_INTERFACE                                  \
    static inline IParseHandler::InterfaceId GetHandlerStaticId()   \
    {                                                               \
        static char _unused{};                                      \
        return (IParseHandler::InterfaceId)&_unused;                \
    }                                                               \
    virtual InterfaceId GetHandlerId() const override               \
    {                                                               \
        return GetHandlerStaticId();                                \
    }

    struct ITypeRegistrar
    {
        FIEA_SERVICE_INTERFACE
        virtual void RegisterType(Fiea::Engine::RTTI::IdType typeId, const ClassDefinition& def) = 0;
    };

    struct ITypeProvider
    {
        FIEA_SERVICE_INTERFACE
        virtual const ClassDefinition* GetClassDefinition(Fiea::Engine::RTTI::IdType typeId) const = 0;
    };

    class ParseWriter : public RTTI
    {
        RTTI_DECLARATIONS(ParseWriter, RTTI);

    protected:
        ParseWriter() = default;
    public:
        ParseWriter(const ParseWriter&) = delete;

        void IncrementDepth();
        void DecrementDepth();
        size_t Depth();

    private:
        size_t _Depth = 0;
    };

    struct IParseHandler
    {
        using InterfaceId = size_t;
        virtual void Begin(ParseWriter& writer) {}
        virtual void End(ParseWriter& writer) {}

        virtual bool EnterKey(ParseWriter& writer, const std::string& key, const Json::Value& value) = 0;
        virtual bool ExitKey(ParseWriter& writer, const std::string& key, const Json::Value& value) = 0;

        virtual InterfaceId GetHandlerId() const = 0;

        virtual ~IParseHandler() = default;
    };

    struct IJsonConfig
    {
        FIEA_SERVICE_INTERFACE;

        virtual void AddHandler(std::unique_ptr<IParseHandler> handler) = 0;
        virtual void RemoveHandler(const IParseHandler::InterfaceId& handlerType) = 0;
    };

    struct IJsonParser
    {
        FIEA_SERVICE_INTERFACE;

        virtual void Deserialize(const std::string& jsonString, ParseWriter& writer) = 0;
        virtual void Deserialize(std::istream& jsonStream, ParseWriter& writer) = 0;
        virtual void DeserializeFieldFromFile(const std::string& filePath, ParseWriter& writer) = 0;
    };
}