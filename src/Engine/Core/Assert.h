#pragma once

#include "Logger.h"
#include <string>

#if defined(_MSC_VER)
#define ENGINE_DEBUGBREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#include <csignal>
#define ENGINE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "ENGINE_DEBUGBREAK not implemented for this platform"
#endif

namespace Engine::Core::Detail {

inline void AssertFailed(const char *expr, const char *file, int line,
                         const char *message) {
  Logger::Error(("Assertion failed: " + std::string(expr) +
                 "\n  File: " + file + ":" + std::to_string(line) +
                 "\n  Message: " + (message ? message : "(none)"))
                    .c_str());
}

} // namespace Engine::Core::Detail

// ENGINE_ASSERT: checked ONLY in debug builds. Condition is not evaluated
// at all in release (NDEBUG) builds — do not put side effects in `expr`.
#if !defined(NDEBUG)
#define ENGINE_ASSERT(expr, message)                                           \
  do {                                                                         \
    if (!(expr)) {                                                             \
      Engine::Core::Detail::AssertFailed(#expr, __FILE__, __LINE__, message);  \
      ENGINE_DEBUGBREAK();                                                     \
    }                                                                          \
  } while (0)
#else
#define ENGINE_ASSERT(expr, message) ((void)0)
#endif

// ENGINE_VERIFY: condition is ALWAYS evaluated, even in release builds.
// Use when `expr` has a required side effect. Only logs/breaks in debug.
#define ENGINE_VERIFY(expr, message)                                           \
  do {                                                                         \
    if (!(expr)) {                                                             \
      Engine::Core::Detail::AssertFailed(#expr, __FILE__, __LINE__, message);  \
      ENGINE_DEBUGBREAK();                                                     \
    }                                                                          \
  } while (0)