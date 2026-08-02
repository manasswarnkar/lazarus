#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Platform/Window.h"

#include "glad/gl.h"

using namespace Engine::Core;
using namespace Engine::Platform;
using namespace Engine::Events;

int main() {
  Logger::Info("Engine starting...");

  bool running = true;

  Window window({"Lazarus Engine", 1280, 720, true});

  window.SetEventCallback([&](Event &e) {
    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent &) {
      Logger::Info("Window close requested.");
      running = false;
      return true;
    });

    // dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent &re) {
    //   Logger::Info(re.ToString().c_str());
    //   return true;
    // });
  });

  while (running && !window.ShouldClose()) {
    Time::Update();

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    window.OnUpdate();
  }

  Logger::Info("Engine shutting down.");
  return 0;
}