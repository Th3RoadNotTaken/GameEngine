#include "Heap.h"
#include "FIEAGameEngine/Types.h"

namespace Fiea::Engine::Memory
{
    Heap::Heap(const char* name, size_t size)
    {
        _HeapName = name;
        _TotalHeapSize = size;
        _UsedHeapSize = 0;
        _NumHeaders = 0;
        _HeapStart = nullptr;

        _RawMemoryLocation = std::malloc(size);
        _HeapStart = new (_RawMemoryLocation) Header(-(static_cast<ptrdiff_t>(size)));

        _HeapEnd = reinterpret_cast<Header*>(reinterpret_cast<std::byte*>(_HeapStart) + sizeof(Header) + size);
    }

    Heap::~Heap()
    {
        
    }

    Heap::Header::Header(ptrdiff_t blockSize)
    {
        _BlockSize = blockSize;
    }

    Heap::Header::~Header()
    {
        
    }

    void* Heap::TryAlloc(Header* current, size_t size)
    {
        if (size % ALIGNMENT != 0)
        {
            size_t quotient = size / ALIGNMENT;
            ++quotient;
            size = ALIGNMENT * quotient;
        }

        size_t remainingSize = 0;
        if (current->_BlockSize < 0 && static_cast<size_t>(-current->_BlockSize) > size)
        {
            // Block is bigger than requested size so split
            remainingSize = -current->_BlockSize - size;

            if (remainingSize > sizeof(Header))
            {
                // We have enough memory to create a second block
                current->_BlockSize = size;

                // Calculate start address of the new block
                void* newBlockAddress = reinterpret_cast<void*>(reinterpret_cast<std::byte*>(current) + sizeof(Header) + size);
                Header* newBlock = new (newBlockAddress) Header(-(static_cast<ptrdiff_t>(remainingSize - sizeof(Header))));
                _UsedHeapSize += size;
                ++_NumHeaders;

                return reinterpret_cast<void*>(reinterpret_cast<std::byte*>(current) + sizeof(Header));
            }
            else
            {
                // We don't have enough memory for a second block
                current->_BlockSize = -current->_BlockSize;
                _UsedHeapSize += current->_BlockSize;
                return reinterpret_cast<void*>(reinterpret_cast<std::byte*>(current) + sizeof(Header));
            }
        }

        else if (current->_BlockSize < 0 && static_cast<size_t>(-current->_BlockSize) == size)
        {
            current->_BlockSize = -current->_BlockSize;
            _UsedHeapSize += size;
            return reinterpret_cast<void*>(reinterpret_cast<std::byte*>(current) + sizeof(Header));
        }

        else return nullptr;
    }

    void* Heap::Alloc(size_t size)
    {
        Header* current = _HeapStart;

        while (current != nullptr)
        {
            if (current->_BlockSize < 0)
            {
                return TryAlloc(current, size);
            }

            current = Next(current);
        }
        return nullptr;
    }

    Heap::Header* Heap::Next(Header* current) const
    {
        if (current != nullptr)
        {
            Header* nextPtr = reinterpret_cast<Header*>(reinterpret_cast<std::byte*>(current) + sizeof(Header) + (current->_BlockSize < 0 ? -current->_BlockSize : current->_BlockSize));
            if (nextPtr < _HeapEnd)
                return nextPtr;
            else
                return nullptr;
        }
        // This case isn't currently hitting but I feel that it may pop up sometime in the future
        else
            return nullptr;
    }

    const char* Heap::GetName() const
    {
        return _HeapName;
    }

    void Heap::Free(void* ptr)
    {
        Header* current = _HeapStart;

        while (current != nullptr)
        {
            if (reinterpret_cast<void*>(reinterpret_cast<std::byte*>(current) + sizeof(Header)) == ptr)
            {
                _UsedHeapSize -= current->_BlockSize;
                current->_BlockSize = -current->_BlockSize;
                break;
            }
            current = Next(current);
        }

        current = _HeapStart;
        Header* next = Next(current);

        while (current != nullptr && next != nullptr)
        {
            if (current->_BlockSize < 0 && next->_BlockSize < 0)
            {
                // Can coalesce
                current->_BlockSize = current->_BlockSize + next->_BlockSize - sizeof(Header);
                --_NumHeaders;
                
                // Reset pointers to top
                current = _HeapStart;
                next = Next(current);
            }
            else
            {
                current = Next(current);
                next = Next(next);
            }
        }
    }

    size_t Heap::Used() const
    {
        return _UsedHeapSize;
    }

    size_t Heap::Available() const
    {
        return _TotalHeapSize - _UsedHeapSize - (sizeof(Header) * _NumHeaders);
    }

    size_t Heap::Overhead() const
    {
        Header* current = _HeapStart;
        size_t overhead = sizeof(Heap);

        while (current != nullptr)
        {
            overhead += sizeof(Header);
            current = Next(current);
        }
        return overhead;
    }

    bool Heap::Contains(const void* ptr) const
    {
        if (ptr == nullptr)return false;
        return ptr >= _HeapStart ? ptr < _HeapEnd ? true : false : false;
    }
}