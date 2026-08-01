#pragma once

#include <chrono>

namespace Engine::Core {

// Stopwatch: measures elapsed wall-clock time for a single interval.
// Useful for profiling, load-time measurement, etc.
class Stopwatch {
public:
  Stopwatch();

  void Reset();
  double ElapsedSeconds() const;
  double ElapsedMilliseconds() const;

private:
  using Clock = std::chrono::steady_clock;
  Clock::time_point m_StartTime;
};

// Time: engine-wide frame timing, updated once per frame by the main loop.
// Static API to match Logger's style — single global timeline for the app.
class Time {
public:
  static void Update();

  static double DeltaTime(); // seconds since last Update()
  static double TotalTime(); // seconds since first Update()
  static uint64_t FrameCount();

private:
  using Clock = std::chrono::steady_clock;

  static Clock::time_point s_StartTime;
  static Clock::time_point s_LastFrameTime;
  static double s_DeltaTime;
  static uint64_t s_FrameCount;
  static bool s_Initialized;
};

} // namespace Engine::Core