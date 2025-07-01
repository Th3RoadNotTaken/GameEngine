#include "Tests.h"
#include "../FIEAGameEngine/src/Memory/Heap.h"
#include "FIEAGameEngine/MemoryService.h"
#include "Support/Foo.h"
#include "FIEAGameEngine/ServiceMgr.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Fiea::Engine::Memory;

namespace Fiea::Engine::Tests
{
    TEST_CLASS(HeapTest)
    {
        TEST_MEMCHECK;

        TEST_METHOD(Alloc)
        {
            //CreateMemService();
            IMemoryService* memService = ServiceManager::ProvideInterface<IMemoryService>();
            Heap* newHeap;

            Assert::ExpectException<FieaAbortException>([&]() {
                newHeap = memService->CreateHeap("Test Heap", 7);
                });

            newHeap = memService->CreateHeap("Test Heap", 128);

            // Testing heap name
            Assert::AreEqual("Test Heap", newHeap->GetName());

            // Testing allocation that does not create a second block
            Heap* heap2 = memService->CreateHeap("Test Heap 2", 128);
            heap2->Alloc(120);
            Assert::AreEqual(128_z, heap2->Used());
            Assert::AreEqual(0_z, heap2->Available());

            // Trying to allocate after heap is full
            heap2->Alloc(10);
            Assert::AreEqual(128_z, heap2->Used());
            Assert::AreEqual(0_z, heap2->Available());

            newHeap->Alloc(16);
            // Allocating the requested bytes since already 8 byte aligned
            Assert::AreEqual(16_z, newHeap->Used());
            Assert::AreEqual(104_z, newHeap->Available());

            newHeap->Alloc(20);
            // Allocating 24 bytes to keep aligned
            Assert::AreEqual(40_z, newHeap->Used());
            Assert::AreEqual(72_z, newHeap->Available());

            // Testing invalid alloc size
            newHeap->Alloc(73_z);
            Assert::AreEqual(40_z, newHeap->Used());
            Assert::AreEqual(72_z, newHeap->Available());

            // Testing complete usage of heap memory
            newHeap->Alloc(72);
            Assert::AreEqual(112_z, newHeap->Used());
            Assert::AreEqual(0_z, newHeap->Available());

            // Testing overhead after allocations
            Assert::AreEqual(sizeof(Heap) + 3 * sizeof(ptrdiff_t), newHeap->Overhead());
        
            // Trying to destroy a null heap
            Assert::ExpectException<FieaAbortException>([&]() {
                memService->DestroyHeap(nullptr);
                });

            memService->DestroyHeap(newHeap);
            memService->DestroyHeap(heap2);
            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(FreeBlock)
        {
            //CreateMemService();
            IMemoryService* memService = ServiceManager::ProvideInterface<IMemoryService>();
            Heap* newHeap = memService->CreateHeap("Test Heap", 128);

            // Testing freeing one block
            void* firstAlloc = newHeap->Alloc(20);
            Assert::AreEqual(24_z, newHeap->Used());
            Assert::AreEqual(96_z, newHeap->Available());
            newHeap->Free(firstAlloc);
            Assert::AreEqual(0_z, newHeap->Used());
            Assert::AreEqual(128_z, newHeap->Available());

            firstAlloc = newHeap->Alloc(20);
            void* secondAlloc = newHeap->Alloc(40);

            newHeap->Free(secondAlloc);
            Assert::AreEqual(24_z, newHeap->Used());
            Assert::AreEqual(96_z, newHeap->Available());

            memService->DestroyHeap(newHeap);
            ServiceManager::ResetServiceMgr();
        }

        TEST_METHOD(Coalesce)
        {
            //CreateMemService();
            IMemoryService* memService = ServiceManager::ProvideInterface<IMemoryService>();
            Heap* newHeap = memService->CreateHeap("Test Heap", 128);

            void* firstAlloc = newHeap->Alloc(24);
            void* secondAlloc = newHeap->Alloc(40);
            void* thirdAlloc = newHeap->Alloc(32);

            newHeap->Free(secondAlloc);
            newHeap->Free(thirdAlloc);

            Assert::AreEqual(24_z, newHeap->Used());
            Assert::AreEqual(96_z, newHeap->Available());

            // Testing contains even after freeing a block
            Assert::IsTrue(newHeap->Contains(secondAlloc));
            Assert::IsFalse(newHeap->Contains(reinterpret_cast<std::byte*>(thirdAlloc) + 1000));

            memService->DestroyHeap(newHeap);
            ServiceManager::ResetServiceMgr();
        }
    };
}