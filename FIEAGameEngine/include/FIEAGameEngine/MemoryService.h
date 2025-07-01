#pragma once
#include "FIEAGameEngine/Types.h"
#include "ServiceMgr.h"
#include "../src/Memory/SystemAllocator.h"

#ifdef FIEA_MEMORY_DEBUG    
    #define FNEW(_al, _type) new (_al, __FILE__, __LINE__) (_type)  
#else
    #define FNEW(_al, _type) new (_al) (_type)  
#endif

#ifdef FIEA_MEMORY_DEBUG
    #define FDELETE(_al, _ptr) \
    std::destroy_at(_ptr); \
    ::operator delete((_ptr), (_al), (__FILE__), (__LINE__));
#else
    #define FDELETE(_al, _ptr) \
    std::destroy_at(_ptr); \
    ::operator delete((_ptr), (_al));
#endif

#ifdef FIEA_MEMORY_DEBUG
    #define FALLOC(_al, _size) _al.Alloc(_size, __FILE__, __LINE__);
#else
    #define FALLOC(_al, _size) _al.Alloc(_size);
#endif

#define FFREE(_al, _ptr) _al.Free(_ptr);

namespace Fiea::Engine::Memory
{
    class Heap;
    class Tracker;
    struct IDefaultAllocator;

    struct Allocator
    {
    public:
        [[nodiscard]]
        void* Alloc(size_t size) const;
        [[nodiscard]]
        void* Alloc(size_t size, const char* file, size_t line) const;
        void Free(void* ptr) const;

        Allocator(const Allocator& other) = default;
        Allocator& operator=(const Allocator& rhs) = default;
        ~Allocator() = default;

    private:
        friend class MemoryService;

        Allocator() = delete;

        Allocator(Heap* heap, const char* label, Tracker* tracker);
        Allocator(Heap* heap, const char* label);

        static inline const size_t MAX_LABEL_SIZE = 24;

        Heap& _Heap;
    #ifdef FIEA_MEMORY_DEBUG
        Tracker& _Tracker;
    #endif
        char _Label[MAX_LABEL_SIZE];
    };

    struct IMemoryService
    {
        FIEA_SERVICE_INTERFACE

        virtual Heap* CreateHeap(const char* name, size_t size) = 0; // remove static CreateHeap method from Heap class and implement here
        virtual void DestroyHeap(Heap* heap) = 0; // remove static DestroyHeap method from Heap class and implement here
    #ifdef FIEA_MEMORY_DEBUG
        virtual size_t CountAllocations(const Heap* heap = nullptr) const = 0;
        virtual void DumpAllocations(std::ostream& output, const Heap* heap = nullptr) const = 0;
    #endif
    };

    struct IAllocatorProvider
    {
        FIEA_SERVICE_INTERFACE

        virtual Allocator GetAllocator(Heap* heap, const char* label = nullptr) = 0; // return an allocator that can allocate from a Heap
        virtual Allocator GetAllocator(const char* heapName, const char* label = nullptr) = 0; // return an allocator that can allocate from a Heap
    #ifdef FIEA_MEMORY_DEBUG
        virtual void PushDefaultAllocator(const Allocator& allocator, const char* file, size_t line) = 0;
        virtual void PopDefaultAllocator(const Allocator& allocator, const char* file, size_t line) = 0;
    #else
        virtual void PushDefaultAllocator(const Allocator&) = 0;
        virtual void PopDefaultAllocator(const Allocator&) = 0;
    #endif
    };
}

#ifdef FIEA_MEMORY_DEBUG
    void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& allocator, const char* file, size_t line);
#else
    void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& allocator);
#endif

#ifdef FIEA_MEMORY_DEBUG
    void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& al, const char* file, size_t line);
#else
    void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& al);
#endif