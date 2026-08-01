#include "Logger.h"
#include <iostream>

namespace {
using Engine::Core::Logger;

const char *ToString(Logger::LogLevel level) {
  switch (level) {
  case Logger::LogLevel::Debug:
    return "DEBUG";
  case Logger::LogLevel::Info:
    return "INFO";
  case Logger::LogLevel::Warning:
    return "WARNING";
  case Logger::LogLevel::Error:
    return "ERROR";
  }

  return "UNKNOWN";
}

} // namespace

namespace Engine::Core {

// IMPLEMENTATIONS
void Logger::Log(LogLevel level, const char *message) {
  std::cout << "[" << ToString(level) << "] " << message << '\n';
}

void Logger::Debug(const char *message) {
  Logger::Log(LogLevel::Debug, message);
}

void Logger::Info(const char *message) { Logger::Log(LogLevel::Info, message); }

void Logger::Warning(const char *message) {
  Logger::Log(LogLevel::Warning, message);
}
void Logger::Error(const char *message) {
  Logger::Log(LogLevel::Error, message);
}
} // namespace Engine::Core
