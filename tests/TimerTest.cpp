#include "Core/Timer.h"
#include <chrono> // IWYU pragma: keep
#include <gtest/gtest.h>
#include <thread>

using namespace Engine::Core;
using namespace std::chrono_literals;

TEST(StopwatchTest, MeasuresElapsedTimeCorrectly) {
  Stopwatch sw;
  std::this_thread::sleep_for(50ms);
  double elapsed = sw.ElapsedMilliseconds();

  EXPECT_GE(elapsed, 45.0);
  EXPECT_LE(elapsed, 200.0); // generous upper bound for slow CI machines
}

TEST(StopwatchTest, ResetRestartsInterval) {
  Stopwatch sw;
  std::this_thread::sleep_for(50ms);
  sw.Reset();
  double elapsed = sw.ElapsedMilliseconds();

  EXPECT_LT(elapsed, 50.0);
}

TEST(TimeTest, DeltaTimeReflectsTimeBetweenUpdates) {
  Time::Update();
  std::this_thread::sleep_for(50ms);
  Time::Update();

  double dt = Time::DeltaTime();
  EXPECT_GE(dt, 0.045);
  EXPECT_LE(dt, 0.2);
}

TEST(TimeTest, FrameCountIncrementsOncePerUpdate) {
  uint64_t before = Time::FrameCount();
  Time::Update();
  Time::Update();
  Time::Update();
  uint64_t after = Time::FrameCount();

  EXPECT_EQ(after - before, 3);
}

TEST(TimeTest, TotalTimeIsMonotonicallyIncreasing) {
  Time::Update();
  double t1 = Time::TotalTime();
  std::this_thread::sleep_for(10ms);
  double t2 = Time::TotalTime();

  EXPECT_GT(t2, t1);
}