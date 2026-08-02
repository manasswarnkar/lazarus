#pragma once

#include <cstdint>
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

  void *GetNativeWindow() const { return m_Window; }

private:
  void Init(const WindowProps &props);
  void Shutdown();

  GLFWwindow *m_Window = nullptr;

  struct WindowData {
    std::string Title;
    uint32_t Width = 0, Height = 0;
    bool VSync = true;
  };
  WindowData m_Data;
};

} // namespace Engine::Platform