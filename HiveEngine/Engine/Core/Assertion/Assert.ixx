module;
#if defined(_MSC_VER)
// MSVC or clang-cl on Windows
#include <intrin.h>
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__)
#include <csignal>
#endif

export module Core.Assert;
import std;



#if defined(_MSC_VER)
#define DEBUG_BREAK() __debugbreak()
#elif defined(__APPLE__) || defined(__linux__) || defined(__unix__)
#define DEBUG_BREAK() raise(SIGTRAP)
#elif defined(__clang__) || defined(__GNUC__)
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK() ((void)0) // fallback
#endif

constexpr bool ASSERTION_ENABLED = true;

export inline void hive_assert(bool condition)
{
    if constexpr (ASSERTION_ENABLED)
    {
        if (!condition)
        {
            DEBUG_BREAK();
            std::abort();
        }
    }
}

export inline void hive_soft_assert(bool condition)
{
    if constexpr (ASSERTION_ENABLED)
    {
        if (!condition)
        {
            DEBUG_BREAK();
        }
    }
}
