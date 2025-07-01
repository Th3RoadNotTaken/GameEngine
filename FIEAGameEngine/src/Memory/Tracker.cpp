#include "Tracker.h"
#include "Heap.h"
#include <iostream>

namespace Fiea::Engine::Memory
{
#ifdef FIEA_MEMORY_DEBUG
    void Tracker::Track(void* ptr, const char* heap, const char* label, const char* file, size_t line)
    {
        if (ptr == nullptr)
            return;

        _Allocations[heap][ptr] = { label, file, line };
    }

    void Tracker::Untrack(void* ptr)
    {
        if (ptr == nullptr) return;

        for (auto itr = _Allocations.begin(); itr != _Allocations.end(); ++itr)
        {
            if (itr->second.find(ptr) != itr->second.end())
            {
                itr->second.erase(ptr);
            }
        }

        /*auto itr = _Allocations.find(ptr);
        if (itr == _Allocations.end())
            return;

        _Allocations.erase(ptr);*/
    }

    size_t Tracker::Count(const char* heap) const
    {
        if (heap != nullptr)
        {
            if (_Allocations.find(heap) != _Allocations.end())
            {
                auto itr = _Allocations.find(heap);
                return itr->second.size();
            }
        }

        size_t NumAllocs = 0;
        for (auto itr = _Allocations.begin(); itr != _Allocations.end(); ++itr)
        {
            NumAllocs += itr->second.size();
        }
        return NumAllocs;
    }

    void Tracker::Report(std::ostream& output, const char* heap) const
    {
        output << "Heap,Label,File,Line\n";

        if (heap != nullptr)
        {
            for (auto itr = _Allocations.begin(); itr != _Allocations.end(); ++itr)
            {
                if (itr->first == heap)
                {
                    for (auto itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
                    {
                        output << itr->first << ",";
                        output << itr2->second._AllocatorLabel << ",";
                        output << itr2->second._FileName << ",";
                        output << itr2->second._LineNumber << ",\n";
                    }
                    return;
                }
            }
        }

        else
        {
            for (auto itr = _Allocations.begin(); itr != _Allocations.end(); ++itr)
            {
                for (auto itr2 = itr->second.begin(); itr2 != itr->second.end(); ++itr2)
                {
                    output << itr->first << ",";
                    output << itr2->second._AllocatorLabel << ",";
                    output << itr2->second._FileName << ",";
                    output << itr2->second._LineNumber << ",\n";
                }
            }
        }
    }
#endif
}