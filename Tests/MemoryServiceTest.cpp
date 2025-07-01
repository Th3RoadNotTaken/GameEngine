#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "FIEAGameEngine/MemoryService.h"
#include "Support/Foo.h"
#include "../FIEAGameEngine/src/Memory/Heap.h"
#include "FIEAGameEngine/ServiceMgr.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Memory;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(MemoryServiceTest)
    {
        TEST_MEMCHECK;

        TEST_METHOD(MacroTest)
        {
            //CreateMemService();
            IMemoryService* memService = ServiceManager::ProvideInterface<IMemoryService>();
            IAllocatorProvider* allocProvider = ServiceManager::ProvideInterface<IAllocatorProvider>();

            Heap* newHeap = memService->CreateHeap("Test Heap", 256);
            Allocator allocator = allocProvider->GetAllocator(newHeap, "Test heap allocator");

            int* a = FNEW(allocator, int);
            *a = 10;
            Assert::AreEqual(10, *a);

            double* f = FNEW(allocator, double);
            *f = 2.3;
            Assert::AreEqual(2.3, *f);

            void* firstAllocation = FALLOC(allocator, 20);
            void* secondAllocation = FALLOC(allocator, 40);
            Assert::AreEqual(64 + sizeof(a) + sizeof(f), newHeap->Used());
            Assert::AreEqual(176 - 4 * sizeof(ptrdiff_t), newHeap->Available());

        #ifdef FIEA_MEMORY_DEBUG
            Assert::AreEqual(4_z, memService->CountAllocations(newHeap));
        #endif 
            
            FFREE(allocator, firstAllocation);
            FFREE(allocator, secondAllocation);

            FDELETE(allocator, a);
            FDELETE(allocator, f);

            memService->DestroyHeap(newHeap);
            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(MultipleHeaps)
        {
            IMemoryService* memService = ServiceManager::ProvideInterface<IMemoryService>();
            IAllocatorProvider* allocProvider = ServiceManager::ProvideInterface<IAllocatorProvider>();

            Heap* heap1 = memService->CreateHeap("Heap 1", 128);
            Heap* heap2 = memService->CreateHeap("Heap 2", 256);

            Allocator allocator1 = allocProvider->GetAllocator(heap1, "Heap 1 allocator");
            Allocator allocator2 = allocProvider->GetAllocator(heap2, "Heap 2 allocator");
            
            void* heap1Alloc1 = FALLOC(allocator1, 20);
            void* heap1Alloc2 = FALLOC(allocator1, 30);

            void* heap2Alloc1 = FALLOC(allocator2, 40);
            void* heap2Alloc2 = FALLOC(allocator2, 50);
            void* heap2Alloc3 = FALLOC(allocator2, 10);

        #ifdef FIEA_MEMORY_DEBUG
            Assert::AreEqual(2_z, memService->CountAllocations(heap1));
            Assert::AreEqual(3_z, memService->CountAllocations(heap2));
            Assert::AreEqual(5_z, memService->CountAllocations());
        #endif

            FFREE(allocator1, heap1Alloc1);

        #ifdef FIEA_MEMORY_DEBUG
            Assert::AreEqual(1_z, memService->CountAllocations(heap1));
            Assert::AreEqual(3_z, memService->CountAllocations(heap2));
            Assert::AreEqual(4_z, memService->CountAllocations());
        #endif

            // Dump allocations for specific heap
    #ifdef FIEA_MEMORY_DEBUG
            std::ofstream outFile("../../HeapDump.csv");
            memService->DumpAllocations(outFile, heap2);

            memService->DumpAllocations(outFile);
    #endif

            memService->DestroyHeap(heap1);
            memService->DestroyHeap(heap2);
            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(PushPopAllocator)
        {
            //CreateMemService();
            IMemoryService* memService = ServiceManager::ProvideInterface<IMemoryService>();
            IAllocatorProvider* allocProvider = ServiceManager::ProvideInterface<IAllocatorProvider>();

            Heap* heap1 = memService->CreateHeap("Heap 1", 100000);

            Allocator allocator1 = allocProvider->GetAllocator(heap1, "Heap 1 allocator");

            #ifdef FIEA_MEMORY_DEBUG
                allocProvider->PushDefaultAllocator(allocator1, __FILE__, __LINE__);

                int* a = new int(3);
                bool* b = new bool(false);

                allocProvider->PopDefaultAllocator(allocator1, __FILE__, __LINE__);
            #else
                allocProvider->PushDefaultAllocator(allocator1);

                int* a = new int(3);
                bool* b = new bool(false);

                allocProvider->PopDefaultAllocator(allocator1);
            #endif

            #ifdef FIEA_MEMORY_DEBUG
                Assert::AreEqual(2_z, memService->CountAllocations());
            #endif

            memService->DestroyHeap(heap1);
            ServiceManager::ResetServiceMgr();
        }
    };
}