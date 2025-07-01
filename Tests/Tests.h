#pragma once

#include <excpt.h>
#include <CppUnitTest.h>
#include "FieaGameEngine\Types.h"
#include "Content/Scope.h"
#include "FIEAGameEngine/GameObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
    template<>
    inline std::wstring ToString<string>(const string& t)
    {
        RETURN_WIDE_STRING(std::wstring(t.begin(), t.end()));
    }

    template<>
    inline std::wstring ToString<string>(const string* t)
    {
        RETURN_WIDE_STRING(std::wstring(t->begin(), t->end()));
    }

    template<>
    inline std::wstring ToString<string>(string* t)
    {
        RETURN_WIDE_STRING(std::wstring(t->begin(), t->end()));
    }

    template<>
    inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
    {
        std::string str = glm::to_string(t);
        return std::wstring(str.begin(), str.end());
    }

    template<>
    inline std::wstring ToString<glm::vec4>(const glm::vec4* t)
    {
        if (t == nullptr) return L"nullptr";
        std::string str = glm::to_string(*t);
        return std::wstring(str.begin(), str.end());
    }

    template<>
    inline std::wstring ToString<glm::vec4>(glm::vec4* t)
    {
        if (t == nullptr) return L"nullptr";
        std::string str = glm::to_string(*t);
        return std::wstring(str.begin(), str.end());
    }

    template<>
    inline std::wstring ToString<glm::mat4x4>(const glm::mat4x4& t)
    {
        std::string str = glm::to_string(t);
        return std::wstring(str.begin(), str.end());
    }

    template<>
    inline std::wstring ToString<glm::mat4x4>(const glm::mat4x4* t)
    {
        if (t == nullptr) return L"nullptr";
        std::string str = glm::to_string(*t);
        return std::wstring(str.begin(), str.end());
    }

    template<>
    inline std::wstring ToString<glm::mat4x4>(glm::mat4x4* t)
    {
        if (t == nullptr) return L"nullptr";
        std::string str = glm::to_string(*t);
        return std::wstring(str.begin(), str.end());
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Scope>(const Fiea::Engine::Content::Scope& t)
    {
        std::wostringstream woss;
        woss << L"Scope: " << &t;
        return woss.str();
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Scope>(const Fiea::Engine::Content::Scope* t)
    {
        return (t != nullptr) ? ToString(*t) : L"Null Scope";
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Scope>(Fiea::Engine::Content::Scope* t)
    {
        return (t != nullptr) ? ToString(*t) : L"Null Scope";
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Datum>(const Fiea::Engine::Content::Datum& t)
    {
        std::wostringstream woss;
        woss << L"Datum: " << &t;
        return woss.str();
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Datum>(const Fiea::Engine::Content::Datum* t)
    {
        return t != nullptr ? ToString(*t) : L"null";
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::Content::Datum>(Fiea::Engine::Content::Datum* t)
    {
        return t != nullptr ? ToString(*t) : L"null";
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::GameObject>(const Fiea::Engine::GameObject& t)
    {
        std::wostringstream woss;
        woss << L"GameObject: " << &t;
        return woss.str();
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::GameObject>(const Fiea::Engine::GameObject* t)
    {
        return (t != nullptr) ? ToString(*t) : L"Null GameObject";
    }

    template<>
    inline std::wstring ToString<Fiea::Engine::GameObject>(Fiea::Engine::GameObject* t)
    {
        return (t != nullptr) ? ToString(*t) : L"Null GameObject";
    }
}

// make it easier to create size_t and ptrdiff_t literals
constexpr std::size_t operator "" _z(unsigned long long n) { return size_t(n); }
constexpr std::ptrdiff_t operator "" _sz(unsigned long long n) { return ptrdiff_t(n); }

namespace Fiea::Engine::Tests
{
    template<typename _FUNCTOR> static uint32_t TestSEH(_FUNCTOR functor)
    {
        __try
        {
            functor();
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            return GetExceptionCode();
        }
        return 0;
    }

    template<uint32_t _CODE, typename _FUNCTOR> static void ExpectSEH(_FUNCTOR functor, const wchar_t* message = NULL, const __LineInfo* pLineInfo = NULL)
    {
        FIEA_ASSERT_HIDE();

        if (_CODE != TestSEH(functor))
        {
            Assert::Fail(message, pLineInfo);
        }
    }
}

#define LOG(_stream)                            \
    {                                           \
        std::ostringstream str;                 \
        str << _stream;                         \
        Logger::WriteMessage(str.str().c_str());\
    }

#ifdef FIEA_DEBUG
#define TEST_MEMCHECK                                                                           \
    public:                                                                                     \
        TEST_METHOD_INITIALIZE(Initialize)                                                      \
        {                                                                                       \
            _CrtMemCheckpoint(&_startMemState);                                                 \
        }                                                                                       \
                                                                                                \
        TEST_METHOD_CLEANUP(Cleanup)                                                            \
        {                                                                                       \
            _CrtMemState endMemState, diffMemState;                                             \
            _CrtMemCheckpoint(&endMemState);                                                    \
            if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))                \
            {                                                                                   \
                _CrtMemDumpStatistics(&diffMemState);                                           \
                Assert::Fail(L"Memory Leaks!");                                                 \
            }                                                                                   \
        }                                                                                       \
    private:                                                                                    \
        inline static _CrtMemState _startMemState
#else
#define TEST_MEMCHECK
#endif