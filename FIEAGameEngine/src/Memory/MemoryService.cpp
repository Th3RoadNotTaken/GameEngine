#include "FIEAGameEngine/Types.h"
#include "FIEAGameEngine/MemoryService.h"
#include "Heap.h"
#include "Tracker.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stack>

namespace Fiea::Engine::Memory
{
    template<typename T, bool CreateIfNull = true >
    class MemoryServiceSingleton : Singleton<T>
    {
    public:
        static void Create() {
            void* ptr = malloc(sizeof(T));
            _Instance = static_cast<T*>(ptr); // Lazy init to avoid circular new(s)
            new(_Instance) T();
        }
        static void Destroy() { delete _Instance; _Instance = nullptr; }
        static T* Instance()
        {
            // FIEA_ASSERT(_Instance != nullptr);
            if (_Instance == nullptr)
            {
                if (CreateIfNull)
                {
                    Create();
                }
            }
            else
            {
                FIEA_ASSERT(_Instance != nullptr);
            }
            return _Instance;
        }
    protected:
        static inline T* _Instance = nullptr;
    };

    struct IDefaultAllocator
    {
        virtual void* DefaultAlloc(size_t size) = 0;
        virtual void DefaultFree(void* ptr) = 0;
    };

    class MemoryService : public IMemoryService, public IAllocatorProvider, public IDefaultAllocator
    {
    private:
        std::unordered_map<
            string,
            Heap*,
            std::hash<string>,
            std::equal_to<string>,
            SystemAllocator<std::pair<const string, Heap*>>> _NameToHeapMap;

        struct AllocInfo
        {
            const Allocator& allocator;
            const char* file;
            size_t line;
        };
    #ifdef FIEA_MEMORY_DEBUG
        std::stack < AllocInfo, std::vector<AllocInfo >> _AllocStack;
    #else
        std::stack<Allocator, std::vector<Allocator>> _AllocStack;
    #endif

    #ifdef FIEA_MEMORY_DEBUG
        Tracker _Tracker;
    #endif  

    FIEA_SERVICE_BIND2(MemoryService, IMemoryService, IAllocatorProvider)

    public:
        static inline IDefaultAllocator* _DefAllocator = nullptr;

        MemoryService()
        {
            _DefAllocator = this;
        }

        ~MemoryService()
        {
            _DefAllocator = nullptr;
        }

        /**
         * @brief Creates a new memory heap.
         * @param name The name of the heap.
         * @param size The size of the heap.
         * @return Pointer to the created heap.
         */
        virtual Heap* CreateHeap(const char* name, size_t size) override
        {
            FIEA_ERROR(size > sizeof(Heap::Header));

            Heap* newHeap = new Heap(name, size);
            _NameToHeapMap[name] = newHeap;
            return newHeap;
        }

        /**
         * @brief Destroys the specified memory heap.
         * @param heap Pointer to the heap to destroy.
         */
        virtual void DestroyHeap(Heap* heap) override
        {
            FIEA_ERROR(heap != nullptr);

            if (heap != nullptr)
            {
                std::free(heap->_RawMemoryLocation);
                delete heap;
            }
        }

        /**
         * @brief Gets an allocator for the specified heap.
         * @param heap Pointer to the heap.
         * @param label Optional label for the allocator.
         * @return Allocator for the specified heap.
         */
        virtual Allocator GetAllocator(Heap* heap, const char* label = nullptr) override
        {
            #ifdef FIEA_MEMORY_DEBUG
                return Allocator(heap, label, &_Tracker);
            #else
                return Allocator(heap, label);
            #endif 
        }
        
        /**
         * @brief Gets an allocator for the specified heap name.
         * @param heapName The name of the heap.
         * @param label Optional label for the allocator.
         * @return Allocator for the specified heap.
         */
        virtual Allocator GetAllocator(const char* heapName, const char* label = nullptr) override
        {
            auto it = _NameToHeapMap.find(heapName);
            FIEA_ERROR(it != _NameToHeapMap.end());

            #ifdef FIEA_MEMORY_DEBUG
                return Allocator(it->second, label, &_Tracker);
            #else
                return Allocator(it->second, label);
            #endif
        }

    #ifdef FIEA_MEMORY_DEBUG
        /**
         * @brief Counts the number of allocations in the specified heap.
         * @param heap Pointer to the heap, or nullptr for all heaps.
         * @return The number of allocations.
         */
        virtual size_t CountAllocations(const Heap* heap = nullptr) const override
        {
            return _Tracker.Count(heap == nullptr ? nullptr : heap->GetName());
        }

        /**
         * @brief Dumps the allocations to the specified output stream.
         * @param output The output stream to write to.
         * @param heap Pointer to the heap, or nullptr for all heaps.
         */
        virtual void DumpAllocations(std::ostream& output, const Heap* heap = nullptr) const override
        {
            output << "Name,Used Bytes,Free Bytes,Overhead Bytes\n";

            if (heap != nullptr)
            {
                for (auto itr = _NameToHeapMap.begin(); itr != _NameToHeapMap.end(); ++itr)
                {
                    if (itr->second == heap)
                    {
                        output << itr->first << ",";
                        output << heap->Used() << ",";
                        output << heap->Available() << ",";
                        output << heap->Overhead() << ",\n";

                        std::ofstream outFile("../../Allocations.csv");
                        _Tracker.Report(outFile, itr->first.c_str());
                        return;
                    }
                }
                return;
            }
            else
            {
                for (auto itr = _NameToHeapMap.begin(); itr != _NameToHeapMap.end(); ++itr)
                {
                    output << itr->first << ",";
                    output << itr->second->Used() << ",";
                    output << itr->second->Available() << ",";
                    output << itr->second->Overhead() << ",\n";
                }
                std::ofstream outFile("../../Allocations.csv");
                _Tracker.Report(outFile);
            }
        }
    #endif

    #ifdef FIEA_MEMORY_DEBUG
        /**
         * @brief Pushes the default allocator onto the stack.
         * @param allocator The allocator to push.
         * @param file The file name where the allocator is pushed (used in debug mode).
         * @param line The line number where the allocator is pushed (used in debug mode).
         */
        virtual void PushDefaultAllocator(const Allocator& allocator, const char* file, size_t line) override
        {
            _AllocStack.push({ allocator, file, line });
        }

        /**
         * @brief Pops the default allocator from the stack.
         * @param allocator The allocator to pop.
         * @param file The file name where the allocator is popped (used in debug mode).
         * @param line The line number where the allocator is popped (used in debug mode).
         */
        virtual void PopDefaultAllocator(const Allocator& allocator, const char* file, size_t line) override
        {
            if (!_AllocStack.empty())
                _AllocStack.pop();
        }
    #else
        /**
         * @brief Pushes the default allocator onto the stack (non-debug).
         * @param allocator The allocator to push.
         */
        virtual void PushDefaultAllocator(const Allocator& allocator) override
        {
            _AllocStack.push(allocator);
        }

        /**
         * @brief Pops the default allocator from the stack (non-debug).
         * @param allocator The allocator to pop (unused).
         */
        virtual void PopDefaultAllocator(const Allocator& allocator) override
        {
            if (!_AllocStack.empty())
                _AllocStack.pop();
        }
    #endif

        /**
         * @brief Allocates memory using the current default allocator or malloc.
         * @param size The size of the memory block to allocate.
         * @return Pointer to the allocated memory.
         */
        virtual void* DefaultAlloc(size_t size) override
        {
            if (_AllocStack.empty())
                return malloc(size);    

            #ifdef FIEA_MEMORY_DEBUG
                void* allocPtr = _AllocStack.top().allocator.Alloc(size);
                _Tracker.Track(allocPtr, _AllocStack.top().allocator._Heap.GetName(), _AllocStack.top().allocator._Label, _AllocStack.top().file, _AllocStack.top().line);
                return allocPtr;
            #else
                return _AllocStack.top().Alloc(size);
            #endif
        }

        /**
         * @brief Frees memory using the current default allocator or free.
         * @param ptr Pointer to the memory block to free.
         */
        virtual void DefaultFree(void* ptr) override
        {
            if (_AllocStack.empty())
            {
                free(ptr);
                return;
            }

            #ifdef FIEA_MEMORY_DEBUG
                Allocator allocator = _AllocStack.top().allocator;
            #else
                Allocator allocator = _AllocStack.top();
            #endif

            if (allocator._Heap.Contains(ptr))
            {
                #ifdef FIEA_MEMORY_DEBUG
                    _Tracker.Untrack(ptr);
                #endif
                allocator._Heap.Free(ptr);
            }
            else
            {
                free(ptr);
            }
        }
    };

    #ifdef FIEA_MEMORY_DEBUG
        /**
         * @brief Constructs an Allocator with debugging information.
         * @param heap The memory heap associated with the allocator.
         * @param label A label for the allocator.
         * @param tracker The memory tracker for debugging.
         */
        Allocator::Allocator(Heap* heap, const char* label, Tracker* tracker) : _Heap(*heap), _Tracker(*tracker)
        {
            strncpy(_Label, label, MAX_LABEL_SIZE);
            _Label[MAX_LABEL_SIZE - 1] = '\0';
        }
    #else
        /**
         * @brief Constructs an Allocator.
         * @param heap The memory heap associated with the allocator.
         * @param label A label for the allocator.
         */
        Allocator::Allocator(Heap* heap, const char* label) : _Heap(*heap)
        {
            strncpy(_Label, label, MAX_LABEL_SIZE);
            _Label[MAX_LABEL_SIZE - 1] = '\0';
        }
    #endif

    /**
    * @brief Allocates memory from the heap.
    * @param size The size of the memory block to allocate.
    * @return Pointer to the allocated memory.
    */
    void* Allocator::Alloc(size_t size) const
    {
        return _Heap.Alloc(size);
    }

    /**
     * @brief Allocates memory with debugging information.
     * @param size The size of the memory block to allocate.
     * @param file The file name where the allocation occurs.
     * @param line The line number where the allocation occurs.
     * @return Pointer to the allocated memory.
     */
    void* Allocator::Alloc(size_t size, const char* file, size_t line) const
    {
        void* allocPtr = _Heap.Alloc(size);

    #ifdef FIEA_MEMORY_DEBUG
        _Tracker.Track(allocPtr, _Heap.GetName(), _Label, file, line);
    #endif

        return allocPtr;
    }

    /**
     * @brief Frees memory in the heap.
     * @param ptr Pointer to the memory block to free.
     */
    void Allocator::Free(void* ptr) const
    {
        _Heap.Free(ptr);
    #ifdef FIEA_MEMORY_DEBUG
        _Tracker.Untrack(ptr);
    #endif
    }
}

/**
 * @brief Overloaded new operator using an allocator.
 * @param size The size of the memory block to allocate.
 * @param allocator The allocator to use.
 * @return Pointer to the allocated memory.
 */
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& allocator)
{
    return allocator.Alloc(size);
}

/**
 * @brief Overloaded new operator with debugging information.
 * @param size The size of the memory block to allocate.
 * @param allocator The allocator to use.
 * @param file The file name where the allocation occurs.
 * @param line The line number where the allocation occurs.
 * @return Pointer to the allocated memory.
 */
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& allocator, const char* file, size_t line)
{
    return allocator.Alloc(size, file, line);
}

/**
 * @brief Overloaded delete operator using an allocator.
 * @param ptr Pointer to the memory block to free.
 * @param allocator The allocator to use.
 */
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& allocator)
{
    allocator.Free(ptr);
}

/**
 * @brief Overloaded delete operator with debugging information.
 * @param ptr Pointer to the memory block to free.
 * @param allocator The allocator to use.
 * @param file The file name where the deallocation occurs.
 * @param line The line number where the deallocation occurs.
 */
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& allocator, const char* file, size_t line)
{
    allocator.Free(ptr);
}

/**
 * @brief Global new operator with default allocator.
 * @param size The size of the memory block to allocate.
 * @return Pointer to the allocated memory.
 */
_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(_Size)
void* operator new(size_t size)
{
    using namespace Fiea::Engine::Memory;

    if (size == 0)
        ++size;

    IDefaultAllocator* defAllocator = MemoryService::_DefAllocator;
    void* allocPtr;
    if (defAllocator != nullptr)
    {
        allocPtr = defAllocator->DefaultAlloc(size);
    }
    else
    {
        allocPtr = malloc(size);
    }

    if (allocPtr == nullptr)
    {
        throw std::bad_alloc();
    }
    else
    {
        return allocPtr;
    }
}

/**
 * @brief Global delete operator with default allocator.
 * @param ptr Pointer to the memory block to free.
 */
void operator delete(void* ptr)
{
    using namespace Fiea::Engine::Memory;

    if (ptr != nullptr)
    {
        IDefaultAllocator* defAllocator = MemoryService::_DefAllocator;
        if (defAllocator != nullptr)
        {
            defAllocator->DefaultFree(ptr);
        }
        else
        {
            free(ptr);
        }
    }
}