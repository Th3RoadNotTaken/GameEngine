#include "FIEAGameEngine/Types.h"
#include "Tests.h"
#include "Support/Foo.h"
#include "FIEAGameEngine/Timer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include <vector>
#include <list>
#include <set>
#include <unordered_set>

#include <ostream>
#include <functional>

namespace Fiea::Engine::Tests
{
    // template meta-programming!
    //
    // start with a template class that produces an always-false boolean
    template< typename C, typename = void >
    struct HasReserve : std::false_type {};

    // using SFINAE, resolve to struct containing a true value, if the reserve method exists!
    template< typename C >
    struct HasReserve<
        C, typename std::enable_if<
        std::is_same<
        decltype(std::declval<C>().reserve(std::declval<typename C::size_type>())),
        void
        >::value
        >::type
    > : std::true_type {
    };

    // using HasReserve, produce a default impl for things that don't have reserve
    template <typename C>
    std::enable_if_t<!HasReserve<C>::value> Reserve(C&, size_t) {}

    // and relying upon SFINAE again, we do the reserve!
    template <typename C>
    std::enable_if_t<HasReserve<C>::value> Reserve(C& c, size_t n)
    {
        c.reserve(n);
    }

    template< typename C, typename = void >
    struct HasPushBack : std::false_type {};

    template< typename C >
    struct HasPushBack <
        C, typename std::enable_if<
        std::is_same<
        decltype(std::declval<C>().push_back(std::declval<typename C::value_type>())),
        void
        >::value
        >::type
    > : std::true_type {
    };

    /**
     * @brief Templated Push_Back function for containers that don't have a push_back function
     * @type C container type
     * @param c The container in which the element will be inserted
     * @param n The element's value that will be inserted into the container
     */
    template <typename C>
    std::enable_if_t<!HasPushBack<C>::value> Push_Back(C& c, const typename C::value_type& n)
    {
        c.insert(c.end(), n);
    }

    /**
     * @brief Templated Push_Back function for containers that have a push_back function
     * @type C container type
     * @param c The container in which the element will be inserted
     * @param n The element's value that will be inserted into the container
     */
    template <typename C>
    std::enable_if_t<HasPushBack<C>::value> Push_Back(C& c, const typename C::value_type& n)
    {
        c.push_back(n);
    }

    template< typename C, typename = void >
    struct HasPushFront : std::false_type {};

    template< typename C >
    struct HasPushFront <
        C, typename std::enable_if<
        std::is_same<
        decltype(std::declval<C>().push_front(std::declval<typename C::value_type>())),
        void
        >::value
        >::type
    > : std::true_type {
    };

    /**
     * @brief Templated Push_Front function for containers that don't have a push_front function
     * @type C container type
     * @param c The container in which the element will be inserted
     * @param n The element's value that will be inserted into the container
     */
    template <typename C>
    std::enable_if_t<!HasPushFront<C>::value> Push_Front(C& c, const typename C::value_type& n)
    {
        c.insert(c.begin(), n);
    }

    /**
     * @brief Templated Push_Front function for containers that have a push_front function
     * @type C container type
     * @param c The container in which the element will be inserted
     * @param n The element's value that will be inserted into the container
     */
    template <typename C>
    std::enable_if_t<HasPushFront<C>::value> Push_Front(C& c, const typename C::value_type& n)
    {
        c.push_front(n);
    }

    template< typename C, typename = void >
    struct HasFind : std::false_type {};

    template< typename C >
    struct HasFind <
        C, typename std::enable_if<
        std::is_same<
        decltype(std::declval<C>().find(std::declval<typename C::value_type>())),
        void
        >::value
        >::type
    > : std::true_type {
    };

    /**
     * @brief Templated Find function for containers that don't have a find() function
     * @type C container type
     * @param c The container in which the element will be inserted
     * @param n The element's value that will be inserted into the container
     */
    template <typename C>
    std::enable_if_t<!HasFind<C>::value, bool> Find(C& c, const typename C::value_type& n)
    {
        return std::find(c.begin(), c.end(), n) != c.end();
    }

    /**
     * @brief Templated Find function for containers that have a find() function
     * @type C container type
     * @param c The container in which the element will be inserted
     * @param n The element's value that will be inserted into the container
     */
    template <typename C>
    std::enable_if_t<HasFind<C>::value, bool> Find(C& c, const typename C::value_type& n)
    {
        return c.find(n) != c.end();
    }

    TEST_CLASS(ContainerTest)
    {
        /**
         * @brief Returns a value of type T
         * @param num The value that needs to be typecasted
         * @return A value that is typecasted to T
         */
        template <typename T>
        static T GetValue(size_t num)
        {
            return static_cast<T>(num);
        }

        /**
         * @brief Specialized template function that returns a value of type string
         * @param num The value that needs to be typecasted
         * @return A value that is typecasted to T
         */
        template<>
        static string GetValue(size_t num)
        {
            std::ostringstream oss;
            oss << num;
            return string(oss.str().begin(), oss.str().end());
        }

        /**
         * @brief Specialized template function that returns a value of type Foo
         * @param num The value that needs to be typecasted
         * @return A value that is typecasted to T
         */
        template<>
        static Foo GetValue(size_t num)
        {
            return Foo(static_cast<int32_t>(num));
        }

        template<template<typename T> typename C, typename E>
        static C<E> FillContainer(size_t numValsToFill)
        {
            C<E> container;
            Reserve(container, numValsToFill);

            for (size_t i = 0; i < numValsToFill; ++i)
            {
                Push_Back(container, GetValue<E>(i));
            }
            return container;
        }

        template<template<typename T> typename C, typename E>
        static void RunBenchmark(std::vector<size_t>& iterationSizes, const size_t& numMeasures, const string& funcName)
        {
            FIEA_ASSERT(iterationSizes.size() != 0);

            C<E> container;

            const size_t numElements = iterationSizes.size();

            for (size_t i = 0; i < numElements; ++i)
            {
                auto filledContainer = FillContainer<C,E>(iterationSizes[i]);
                Reserve(filledContainer, iterationSizes[i] + numMeasures);

                Timer t;

                for (size_t j = iterationSizes[i]; j < iterationSizes[i] + numMeasures; ++j)
                {
                    if (funcName == "Push_Back")
                        Push_Back(filledContainer, GetValue<E>(j));
                    else if (funcName == "Push_Front")
                        Push_Front(filledContainer, GetValue<E>(j));
                    else
                        Find(filledContainer, GetValue<E>(j));
                }

                double ms = t.Elapsed();
                LOG("Function: " << funcName << std::endl << "Size: " << iterationSizes[i] << " Time Taken : " << ms << " ms" << std::endl);
            }
        }

        template<template<typename T> typename C, typename E>
        static void RunAllBenchmarks(std::vector<size_t>& iterationSizes, const size_t& numMeasures)
        {
            string funcName;

            for (size_t i = 0; i < 3; ++i)
            {
                funcName = i == 0 ? "Push_Back" : i == 1 ? "Push_Front" : "Find";
                RunBenchmark<C, E>(iterationSizes, numMeasures, funcName);
            }
        }

        //TEST_MEMCHECK;

        TEST_METHOD(Method)
        {
            std::vector<size_t> sizes = { 0, 10, 100, 1000, 10000, 100000, 1000000, 10000000 };
            const size_t numMeasures = 10;

            //RunAllBenchmarks<std::unordered_set, string>(sizes, numMeasures);
        }
    };
}