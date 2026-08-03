#pragma once

#include "Events/Event.h"
#include <cstdint>
#include <functional>
#include <string>

struct GLFWwindow;

namespace Engine::Platform {

struct WindowProps {
  std::string Title = "Engine";
  uint32_t Width = 1280;
  uint32_t Height = 720;
  bool VSync = true;
};

// RAII wrapper around a GLFW window + OpenGL context.
class Window {
public:
  using EventCallbackFn = std::function<void(Events::Event &)>;

  explicit Window(const WindowProps &props = WindowProps());
  ~Window();

  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  // Call once per frame: processes OS/input events and presents the frame.
  void OnUpdate();

  bool ShouldClose() const;

  uint32_t GetWidth() const { return m_Data.Width; }
  uint32_t GetHeight() const { return m_Data.Height; }

  void SetVSync(bool enabled);
  bool IsVSync() const { return m_Data.VSync; }

  void SetEventCallback(const EventCallbackFn &callback) {
    m_Data.EventCallback = callback;
  }

  // Locks rendering to a fixed aspect ratio. On resize, the OpenGL
  // viewport is letterboxed/pillarboxed (black bars) rather than
  // stretched, so the rendered content's proportions never distort.
  // Pass 0.0f (default) to disable locking and fill the whole window.
  void LockAspectRatio(float aspectRatio);

  void *GetNativeWindow() const { return m_Window; }

private:
  void Init(const WindowProps &props);
  void Shutdown();

  GLFWwindow *m_Window = nullptr;

  struct WindowData {
    std::string Title;
    uint32_t Width = 0, Height = 0;
    bool VSync = true;
    float TargetAspectRatio = 0.0f; // 0 = unlocked, fill window
    EventCallbackFn EventCallback;
  };
  WindowData m_Data;
};

} // namespace Engine::Platform