#include "Timer.h"

namespace Engine::Core {
// ---------- Stopwatch ----------

Stopwatch::Stopwatch() { Reset(); }

void Stopwatch::Reset() { m_StartTime = Clock::now(); }

double Stopwatch::ElapsedSeconds() const {
  return std::chrono::duration<double>(Clock::now() - m_StartTime).count();
}

double Stopwatch::ElapsedMilliseconds() const {
  return std::chrono::duration<double, std::milli>(Clock::now() - m_StartTime)
      .count();
}

// ---------- Time ----------

Time::Clock::time_point Time::s_StartTime;
Time::Clock::time_point Time::s_LastFrameTime;
double Time::s_DeltaTime = 0.0;
uint64_t Time::s_FrameCount = 0;
bool Time::s_Initialized = false;

void Time::Update() {
  const Clock::time_point now = Clock::now();

  if (!s_Initialized) {
    s_StartTime = now;
    s_LastFrameTime = now;
    s_Initialized = true;
  }

  s_DeltaTime = std::chrono::duration<double>(now - s_LastFrameTime).count();
  s_LastFrameTime = now;
  ++s_FrameCount;
}

double Time::DeltaTime() { return s_DeltaTime; }

double Time::TotalTime() {
  return std::chrono::duration<double>(Clock::now() - s_StartTime).count();
}

uint64_t Time::FrameCount() { return s_FrameCount; }

} // namespace Engine::Core