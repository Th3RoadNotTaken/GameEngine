#pragma once
#include "../src/Memory/SystemAllocator.h"

#if defined(_DEBUG) && !defined(FIEA_DEBUG)
#error FIEA_DEBUG should be defined in project settings for debug builds!
#endif

#ifndef FIEA_MEMORY_DEBUG
#ifdef FIEA_DEBUG
#define FIEA_MEMORY_DEBUG
#endif
#endif

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdbool>
#include <string>
#include <crtdefs.h>

using string = std::basic_string<char, std::char_traits<char>, Fiea::Engine::Memory::SystemAllocator<char>>;

// Macros to forcibly crash the application when encountering fatal errors
struct FieaAbortException : public std::exception {
    FieaAbortException(const char* str = nullptr) : std::exception(str) {}
};

// Macros to forcibly crash the application when encountering fatal errors
#define FIEA_ABORT_CODE 0xE0000001
//#define FIEA_ABORT() RaiseException(FIEA_ABORT_CODE, 0, 0, 0)
#define FIEA_ABORT() throw FieaAbortException()
#define FIEA_ABORT_STR(_str) throw FieaAbortException(_str)
#define FIEA_ABORT_CHECK(_code) (GetExceptionCode() == FIEA_ABORT_CODE)

// Completely unnecessary remapping to make it easier to nav to windows exception codes!
#define FIEA_ACCESS_VIOLATION EXCEPTION_ACCESS_VIOLATION

#ifdef FIEA_DEBUG
#define FIEA_ASSERT_HIDE() Fiea::Engine::ScopedAssertHide _unused_hide_
#define FIEA_ASSERT(_expr) if(!(_expr)) _RPT0(_CRT_ASSERT, "Assert: #_expr")
#define FIEA_WARN(_expr) if(!(_expr)) _RPT0(_CRT_WARN, "Warning: #_expr")
//#define FIEA_ERROR(_expr) if(!(_expr)) { _RPT1(_CRT_ERROR, "Error: %s", #_expr); FIEA_ABORT(); }
#define FIEA_ERROR(_expr) if(!(_expr)) { FIEA_ABORT_STR("Error: #_expr"); }
#else
#define FIEA_ASSERT_HIDE()
#define FIEA_ASSERT(_expr)
#define FIEA_WARN(_expr)
//#define FIEA_ERROR(_expr) if(!(_expr)) FIEA_ABORT()
#define FIEA_ERROR(_expr) if(!(_expr)) FIEA_ABORT()
#endif

namespace Fiea::Engine
{
#ifdef FIEA_DEBUG
    struct ScopedAssertHide
    {
        ScopedAssertHide() 
        { 
            _assertMode = _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
            _warnMode   = _CrtSetReportMode(_CRT_WARN,   _CRTDBG_MODE_DEBUG);
            _errorMode  = _CrtSetReportMode(_CRT_ERROR,  _CRTDBG_MODE_DEBUG);
        }
        ~ScopedAssertHide()
        { 
            _CrtSetReportMode(_CRT_ASSERT, _assertMode); 
            _CrtSetReportMode(_CRT_WARN,   _warnMode);
            _CrtSetReportMode(_CRT_ERROR,  _errorMode);
        }
    private:
        uint32_t _assertMode;
        uint32_t _warnMode;
        uint32_t _errorMode;
    };
#endif

    template <typename T>
    struct Singleton
    {
        static void Create() { if (_Instance == nullptr) _Instance = new T; }
        static void Destroy() { delete _Instance; _Instance = nullptr; }
        static T* Instance() { 
            //FIEA_ASSERT(_Instance != nullptr);  
            if (_Instance == nullptr)
            {
                Create();
            }
            return _Instance; 
        }
    protected:
        static inline T* _Instance = nullptr;
    };
}