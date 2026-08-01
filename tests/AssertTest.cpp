#include "Core/Assert.h"
#include <gtest/gtest.h>

using namespace Engine::Core;

// ---------- Passing case: safe to test directly ----------

TEST(AssertTest, PassingConditionDoesNotAbort) {
  int x = 5;
  EXPECT_NO_FATAL_FAILURE(ENGINE_ASSERT(x == 5, "should not fire"));
}

TEST(AssertTest, PassingConditionDoesNotAlterControlFlow) {
  int callCount = 0;

  auto increment = [&]() {
    ENGINE_ASSERT(true, "always passes");
    ++callCount;
  };

  increment();
  increment();

  EXPECT_EQ(callCount, 2);
}

TEST(VerifyTest, PassingConditionDoesNotAbort) {
  bool sideEffectRan = false;

  auto sideEffect = [&]() {
    sideEffectRan = true;
    return true;
  };

  EXPECT_NO_FATAL_FAILURE(ENGINE_VERIFY(sideEffect(), "should not fire"));
  EXPECT_TRUE(sideEffectRan); // confirms VERIFY always evaluates expr
}

// ---------- Failing case: requires a death test ----------
// ENGINE_ASSERT triggers ENGINE_DEBUGBREAK() (SIGTRAP) on failure, which
// would kill the whole test binary if called directly. GoogleTest's death
// tests run the code in a forked subprocess so the crash is isolated and
// verified, instead of taking down the rest of the suite.
//
// NOTE: only meaningful in debug builds — ENGINE_ASSERT compiles to a
// no-op under NDEBUG, so this test is skipped in Release.

#if !defined(NDEBUG)

TEST(AssertDeathTest, FailingConditionAborts) {
  EXPECT_DEATH(
      { ENGINE_ASSERT(false, "intentional failure for death test"); },
      "Assertion failed");
}

TEST(VerifyDeathTest, FailingConditionAborts) {
  EXPECT_DEATH(
      { ENGINE_VERIFY(false, "intentional failure for death test"); },
      "Assertion failed");
}

#endif // !NDEBUG