#pragma once
#include <string>

namespace Fiea::Engine::Memory
{
    class Heap
    {
    private:
        Heap(const char* name, size_t size);
        ~Heap();
        Heap(const Heap& heapToCopy) = delete;
        Heap& operator=(const Heap& heap) = delete;

        struct Header
        {
        public:
            ptrdiff_t _BlockSize;

            Header(ptrdiff_t blockSize);
            ~Header();
        };

        const char* _HeapName;
        size_t _TotalHeapSize;
        size_t _UsedHeapSize;
        size_t _NumHeaders;
        // 8 byte aligned
        static const size_t ALIGNMENT = 8;
        void* _RawMemoryLocation;
        Header* _HeapStart;
        Header* _HeapEnd;
        void* TryAlloc(Header* current, size_t size);
        Header* Next(Header* current) const;
        //Header _Start;

    public:
        const char* GetName() const;
        void* Alloc(size_t size);
        void Free(void* ptr);
        size_t Used() const;
        size_t Available() const;
        size_t Overhead() const;
        bool Contains(const void* ptr) const;

        friend class MemoryService;
    };
}