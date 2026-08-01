#pragma once

namespace Engine::Core {
class Logger {
public:
  enum class LogLevel { Debug, Info, Warning, Error };

  static void Debug(const char *message);
  static void Info(const char *message);
  static void Warning(const char *message);
  static void Error(const char *message);

private:
  static void Log(LogLevel level, const char *message);
};
} // namespace Engine::Core