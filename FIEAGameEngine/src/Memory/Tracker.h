#pragma once
#include "FIEAGameEngine/Types.h"
#include "SystemAllocator.h"
#include <unordered_map>
#include <vector>

namespace Fiea::Engine::Memory
{
#ifdef FIEA_MEMORY_DEBUG
    class Tracker
    {
    public:
        void Track(void* ptr, const char* heap, const char* label, const char* file, size_t line);
        void Untrack(void* ptr);
        size_t Count(const char* heap = nullptr) const;
        void Report(std::ostream& output, const char* heap = nullptr) const;

    private:
        struct DebugMemoryInfo
        {
            //void* _AllocPtr;
            string _AllocatorLabel;
            string _FileName;
            size_t _LineNumber;
        };


        //std::unordered_map<string, std::unordered_map<void*, DebugMemoryInfo>> _Allocations;

        std::unordered_map <
            string,
            std::unordered_map<void*, DebugMemoryInfo, std::hash<void*>, std::equal_to<void*>, SystemAllocator<std::pair<void* const, DebugMemoryInfo>>>,
            std::hash<string>,
            std::equal_to<string>,
            SystemAllocator < std::pair<const string, std::unordered_map<void*, DebugMemoryInfo, std::hash<void*>, std::equal_to<void*>, SystemAllocator<std::pair<void* const, DebugMemoryInfo>>>>>> _Allocations;
    };
#endif
}