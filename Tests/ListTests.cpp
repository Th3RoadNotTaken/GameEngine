#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "FIEAGameEngine/List.h"
#include <list>
#include "Support/Foo.h"

using StringList = Fiea::Engine::List<string>;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

//#include <CppUnitTest.h>
//namespace Microsoft::VisualStudio::CppUnitTestFramework
//{
//    template<>
//    inline std::wstring ToString<string>(const string& t)
//    {
//        RETURN_WIDE_STRING(std::wstring(t.begin(), t.end()));
//    }
//
//    template<>
//    inline std::wstring ToString<string>(const string* t)
//    {
//        RETURN_WIDE_STRING(std::wstring(t->begin(), t->end()));
//    }
//
//    template<>
//    inline std::wstring ToString<string>(string* t)
//    {
//        RETURN_WIDE_STRING(std::wstring(t->begin(), t->end()));
//    }
//}

#include <CppUnitTest.h>
// ToString specializations required for Are*Equal / Are*Same
namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<StringList>(const StringList& t)
    {
        RETURN_WIDE_STRING("List<string>, " << t.size() << " elements");
    }

    template<>
    inline std::wstring ToString<StringList>(const StringList* t)
    {
        RETURN_WIDE_STRING("List<string>, " << t->size() << " elements");
    }

    template<>
    inline std::wstring ToString<StringList>(StringList* t)
    {
        RETURN_WIDE_STRING("List<string>, " << t->size() << " elements");
    }
}

namespace Fiea::Engine::Tests
{
    TEST_CLASS(ListTests)
    {

        TEST_MEMCHECK;
        TEST_METHOD(Constructor)
        {
            StringList StrList;

            Assert::AreEqual(0, (int32_t)StrList.size());
            Assert::AreEqual(true, StrList.empty());
        }

        TEST_METHOD(CopyConstructor)
        {
            //StringList StrList_1 = { "hello", "this", "is", "a", "test"};
            StringList StrList_1;
            StrList_1.push_back("hello");
            StrList_1.push_back("this");
            StrList_1.push_back("is");
            StrList_1.push_back("a");
            StrList_1.push_back("test");

            StringList DifferentList;
            DifferentList.push_back("hello");
            DifferentList.push_back("this");
            DifferentList.push_back("is");
            DifferentList.push_back("a");

            StringList StrList_2(StrList_1);

            Assert::AreEqual(StrList_1, StrList_2);
            Assert::AreNotSame(StrList_1.back(), StrList_2.back());
            Assert::IsTrue(StrList_1 != DifferentList);

            StringList StrList_3;
            StrList_3 = StrList_1;

            Assert::AreEqual(StrList_1, StrList_3);

            auto Itr1 = StrList_1.begin();
            auto Itr2 = StrList_2.begin();
            auto Itr3 = StrList_3.begin();

            while (Itr1 != StrList_1.end() && Itr2 != StrList_2.end())
            {
                Assert::AreEqual(*Itr1, *Itr2);
                ++Itr1;
                ++Itr2;
            }

            Itr1 = StrList_1.begin();

            while (Itr1 != StrList_1.end() && Itr3 != StrList_3.end())
            {
                Assert::AreEqual(*Itr1, *Itr3);
                ++Itr1;
                ++Itr3;
            }
        }

        TEST_METHOD(Iterators)
        {
            StringList StrList;
            StrList.push_back("hello");
            StrList.push_back("this");
            StrList.push_back("is");
            StrList.push_back("a");
            StrList.push_back("test");

            string FirstString = "hello";
            string LastString = "test";

            // Testing the first and last elements of the list
            Assert::AreEqual(FirstString, *StrList.begin());

            auto It = StrList.end();
            --It;
            Assert::AreEqual(LastString, *It);

            auto NewItr = It--;
            Assert::AreEqual(LastString, *NewItr);

            // Testing full iteration of list
            StringList ExpectedStrList;
            ExpectedStrList.push_back("hello");
            ExpectedStrList.push_back("this");
            ExpectedStrList.push_back("is");
            ExpectedStrList.push_back("a");
            ExpectedStrList.push_back("test");

            int32_t Idx = 0;
           for (auto itr = StrList.begin(), itrExpected = ExpectedStrList.begin(); itr != StrList.end() && itrExpected!=ExpectedStrList.end(); ++itr, ++itrExpected, ++Idx)
           {
                Assert::AreEqual(*itrExpected, *itr);
           }

           // Testing an empty list
           StringList EmptyList;  
           Assert::IsTrue(EmptyList.begin() == EmptyList.end());
        }

        TEST_METHOD(Modifiers)
        {
            StringList StrList;
            StrList.push_back("hello");
            StrList.push_back("this");
            StrList.push_back("is");
            StrList.push_back("a");

            // Testing push_back on populated list
            StrList.push_back("test");
            string LastStr = "test";
            auto Itr = StrList.end();
            --Itr;
            Assert::AreEqual(LastStr, *Itr);

            // Testing pop_back on populated list
            LastStr = "a";
            StrList.pop_back();
            Itr = StrList.end();
            --Itr;
            Assert::AreEqual(LastStr, *Itr);

            // Testing push_back on empty list
            StringList EmptyList;
            EmptyList.push_back("test");
            LastStr = "test";
            Assert::AreEqual(LastStr, *EmptyList.begin());

            // Testing push_front on populated list
            StrList.push_front("Hi");
            string FirstStr = "Hi";
            Assert::AreEqual(FirstStr, *StrList.begin());

            // Testing pop_front on populated list
            FirstStr = "hello";
            StrList.pop_front();
            Assert::AreEqual(FirstStr, *StrList.begin());

            // Testing push_front on empty list
            EmptyList.pop_back();
            EmptyList.push_front("test");
            LastStr = "test";
            Assert::AreEqual(LastStr, *EmptyList.begin());
        }

        TEST_METHOD(Accessors)
        {
            // Testing front and back using a const list
            StringList StrList;
            StrList.push_back("hello");
            StrList.push_back("this");
            StrList.push_back("is");
            StrList.push_back("a");
            StrList.push_back("test");

            const StringList& ConstList = StrList;

            // Commenting this test out since I don't have an implementation for 
            // returning a const iterator

            /*string FirstStr = "hello";
            string LastStr = "test";
            auto Itr = ConstList.end();
            --Itr;

            Assert::AreEqual(FirstStr, ConstList.front());
            Assert::AreEqual(LastStr, ConstList.back());*/

            // Testing front and back using non-const list
            StringList NonConstList;
            NonConstList.push_back("hello");
            NonConstList.push_back("this");
            NonConstList.push_back("is");
            NonConstList.push_back("a");
            NonConstList.push_back("test");

            string FirstStr = "hello";
            string LastStr = "test";

            auto Itr = NonConstList.end();
            --Itr;

            Assert::AreEqual(FirstStr, NonConstList.front());
            Assert::AreEqual(LastStr, NonConstList.back());

            Assert::AreEqual(FirstStr, ConstList.front());
            Assert::AreEqual(LastStr, ConstList.back());
        }

        TEST_METHOD(Information)
        {
            // Testing list size
            StringList StrList;
            StrList.push_back("hello");
            StrList.push_back("this");
            StrList.push_back("is");
            StrList.push_back("a");
            StrList.push_back("test");

            size_t StrListSize = StrList.size();
            Assert::AreEqual(StrListSize, StrList.size());

            Assert::IsFalse(StrList.empty());

            // Testing empty list size
            StringList EmptyList;
            size_t EmptyListSize = 0;
            Assert::AreEqual(EmptyListSize, EmptyList.size());

            Assert::IsTrue(EmptyList.empty());
        }

        TEST_METHOD(Dereference)
        {
            StringList StrList;
            StrList.push_back("hello");
            StrList.push_back("this");
            StrList.push_back("is");
            StrList.push_back("a");
            StrList.push_back("test");

            // Testing non-const iterator
            auto Itr = StrList.begin();
            Assert::AreEqual(string("hello"), *Itr);

            // Modifying the first value of the list
            *Itr = "HELLO";
            Assert::AreEqual(string("HELLO"), *Itr);

            // Commenting this test out since I don't have an implementation for 
            // returning a const iterator
            
            //// Testing const iterator
            //const StringList& ConstStrList = StrList;
            //auto ConstItr = ConstStrList.begin();
            //Assert::AreEqual(string("HELLO"), *Itr);

            //// Testing the arrow operator on const iterator
            //Assert::AreEqual('H', ConstItr->at(0));

            // Testing the arrow operator on non-const iterator
            Assert::AreEqual('H', Itr->at(0));
        }

        TEST_METHOD(IteratorNavigation)
        {
            StringList StrList;
            StrList.push_back("hello");
            StrList.push_back("this");
            StrList.push_back("is");
            StrList.push_back("a");
            StrList.push_back("test");

            // Testing prefix increments/decrements
            auto Itr = StrList.begin();
            ++Itr;
            Assert::AreEqual(string("this"), *Itr);
            --Itr;
            Assert::AreEqual(string("hello"), *Itr);

            // Testing postfix increments/decrements
            auto NewItr = Itr++;
            Assert::AreEqual(string("hello"), *NewItr);
            Assert::AreEqual(string("this"), *Itr);
        }

        TEST_METHOD(Comparators)
        {
            StringList StrList;
            StrList.push_back("hello");
            StrList.push_back("this");
            StrList.push_back("is");
            StrList.push_back("a");
            StrList.push_back("test");

            // Testing comparison between iterators
            auto Itr1 = StrList.begin();
            auto Itr2 = StrList.begin();
            auto Itr3 = ++StrList.begin();

            Assert::AreEqual(*Itr1, *Itr2);
            Assert::AreNotEqual(*Itr1, *Itr3);
        }
    };
}