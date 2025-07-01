#pragma once
#include "FIEAGameEngine/Types.h"
#include <unordered_map>

#define FIEA_SERVICE_INTERFACE                                  \
static inline ServiceManager::InterfaceId GetInterfaceId()      \
{                                                               \
    static char _unused{};                                      \
    return (ServiceManager::InterfaceId)&_unused;               \
}
                                                                
#define FIEA_SERVICE_BIND2(_impl, _intf1, _intf2)                                                        \
static bool _Register() {                                                                                      \
    CreateFunc createService = []() -> void* {                                                          \
        return reinterpret_cast<void*>(new _impl);                                                      \
    };                                                                                                  \
    DestroyFunc destroyService = [](void* ptr) {                                                        \
        delete reinterpret_cast<_impl*>(ptr);                                                           \
    };                                                                                                  \
    ProvideFunc provideInterface1 = [](void* ptr) -> void* {                                            \
        _impl* _implPtr = reinterpret_cast<_impl*>(ptr);                                                \
        return static_cast<_intf1*>(_implPtr);                                                          \
    };                                                                                                  \
    ProvideFunc provideInterface2 = [](void* ptr) -> void* {                                            \
        _impl* _implPtr = reinterpret_cast<_impl*>(ptr);                                                \
        return static_cast<_intf2*>(_implPtr);                                                          \
    };                                                                                                  \
    ServiceManager::RegisterInterfaceProvider<_intf1>(createService, destroyService, provideInterface1);\
    ServiceManager::RegisterInterfaceProvider<_intf2>(createService, destroyService, provideInterface2);\
    return true;                                                                                        \
}                                                                                                       \
static inline bool _registered = _Register();                                                           \

#define FIEA_SERVICE_BIND1(_impl, _intf1)                                                              \
static bool _Register() {                                                                              \
    CreateFunc createService = []() -> void* {                                                         \
        return reinterpret_cast<void*>(new _impl);                                                     \
    };                                                                                                 \
    DestroyFunc destroyService = [](void* ptr) {                                                       \
        delete reinterpret_cast<_impl*>(ptr);                                                          \
    };                                                                                                 \
    ProvideFunc provideInterface1 = [](void* ptr) -> void* {                                           \
        _impl* _implPtr = reinterpret_cast<_impl*>(ptr);                                               \
        return static_cast<_intf1*>(_implPtr);                                                         \
    };                                                                                                 \
    ServiceManager::RegisterInterfaceProvider<_intf1>(createService, destroyService, provideInterface1);\
    return true;                                                                                       \
}                                                                                                      \
static inline bool _registered = _Register();                                                          \

#define FIEA_SERVICE_BIND4(_impl, _intf1, _intf2, _intf3, _intf4)                                       \
static bool _Register() {                                                                               \
    CreateFunc createService = []() -> void* {                                                          \
        return reinterpret_cast<void*>(new _impl);                                                      \
    };                                                                                                  \
    DestroyFunc destroyService = [](void* ptr) {                                                        \
        delete reinterpret_cast<_impl*>(ptr);                                                           \
    };                                                                                                  \
    ProvideFunc provideInterface1 = [](void* ptr) -> void* {                                            \
        _impl* _implPtr = reinterpret_cast<_impl*>(ptr);                                                \
        return static_cast<_intf1*>(_implPtr);                                                          \
    };                                                                                                  \
    ProvideFunc provideInterface2 = [](void* ptr) -> void* {                                            \
        _impl* _implPtr = reinterpret_cast<_impl*>(ptr);                                                \
        return static_cast<_intf2*>(_implPtr);                                                          \
    };                                                                                                  \
    ProvideFunc provideInterface3 = [](void* ptr) -> void* {                                            \
        _impl* _implPtr = reinterpret_cast<_impl*>(ptr);                                                \
        return static_cast<_intf3*>(_implPtr);                                                          \
    };                                                                                                  \
    ProvideFunc provideInterface4 = [](void* ptr) -> void* {                                            \
        _impl* _implPtr = reinterpret_cast<_impl*>(ptr);                                                \
        return static_cast<_intf4*>(_implPtr);                                                          \
    };                                                                                                  \
    ServiceManager::RegisterInterfaceProvider<_intf1>(createService, destroyService, provideInterface1);\
    ServiceManager::RegisterInterfaceProvider<_intf2>(createService, destroyService, provideInterface2);\
    ServiceManager::RegisterInterfaceProvider<_intf3>(createService, destroyService, provideInterface3);\
    ServiceManager::RegisterInterfaceProvider<_intf4>(createService, destroyService, provideInterface4);\
    return true;                                                                                        \
}                                                                                                       \
static inline bool _registered = _Register();

namespace Fiea::Engine
{
    using CreateFunc = void* (*) (void);
    using DestroyFunc = void (*) (void*);
    using ProvideFunc = void* (*) (void*);

    struct ServiceProvider
    {
        CreateFunc Create;
        DestroyFunc Destroy;
        ProvideFunc Provide;
    };

    class ServiceManager : public Singleton<ServiceManager>
    {
    public:
        using InterfaceId = size_t;

        template <typename I>
        static void RegisterInterfaceProvider(CreateFunc create, DestroyFunc destroy, ProvideFunc provide);
        template<typename I>
        static I* ProvideInterface();
        static void ResetServiceMgr();

    private:
        std::unordered_map<InterfaceId, ServiceProvider> _ServiceMap;
        std::unordered_map<CreateFunc, void*> _CreatedServiceMap;
    };

    template<typename I>
    static void ServiceManager::RegisterInterfaceProvider(CreateFunc create, DestroyFunc destroy, ProvideFunc provide)
    {
        ServiceManager::Create();
        _Instance->_ServiceMap[I::GetInterfaceId()] = {create, destroy, provide};
    }

    template<typename I>
    static I* ServiceManager::ProvideInterface()
    {
        // Interface not registerered
        FIEA_ERROR(_Instance->_ServiceMap.find(I::GetInterfaceId()) != _Instance->_ServiceMap.end());

        if (_Instance->_CreatedServiceMap.find(_Instance->_ServiceMap[I::GetInterfaceId()].Create) != _Instance->_CreatedServiceMap.end())
        {
            // Interface has been created before
            void* ptr = _Instance->_CreatedServiceMap[_Instance->_ServiceMap[I::GetInterfaceId()].Create];
            return reinterpret_cast<I*>(_Instance->_ServiceMap[I::GetInterfaceId()].Provide(ptr));
        }
        else
        {
            void* ptr = _Instance->_ServiceMap[I::GetInterfaceId()].Create();
            _Instance->_CreatedServiceMap[_Instance->_ServiceMap[I::GetInterfaceId()].Create] = ptr;
            return reinterpret_cast<I*>(_Instance->_ServiceMap[I::GetInterfaceId()].Provide(ptr));
        }
    }

    inline void ServiceManager::ResetServiceMgr()
    {
        for (auto itr = _Instance->_ServiceMap.begin(); itr != _Instance->_ServiceMap.end(); ++itr)
        {
            if (_Instance->_CreatedServiceMap.find(itr->second.Create) != _Instance->_CreatedServiceMap.end())
            {
                itr->second.Destroy(_Instance->_CreatedServiceMap[itr->second.Create]);
                _Instance->_CreatedServiceMap.erase(itr->second.Create);
            }
        }

        std::unordered_map<InterfaceId, ServiceProvider> EmptyServiceMap;
        std::unordered_map<CreateFunc, void*> EmptyCreatedServiceMap;

        std::swap(_Instance->_CreatedServiceMap, EmptyCreatedServiceMap);
        //std::swap(_Instance->_ServiceMap, EmptyServiceMap);
    }
}