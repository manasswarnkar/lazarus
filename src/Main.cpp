#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Platform/Window.h"

#include "glad/gl.h"

using namespace Engine::Core;
using namespace Engine::Platform;

int main() {
  Logger::Info("Engine starting...");

  Window window({"Lazarus Engine", 1280, 720, true});

  while (!window.ShouldClose()) {
    Time::Update();

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window.OnUpdate();
  }

  Logger::Info("Engine shutting down.");

  return 0;
}