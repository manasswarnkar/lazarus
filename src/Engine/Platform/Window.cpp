#include "Window.h"
#include "Core/Assert.h"
#include "Core/Logger.h"
#include "Core/ViewportMath.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Input/Input.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace {
void GLFWErrorCallback(int error, const char *description) {
  Engine::Core::Logger::Error((std::string("GLFW Error (") +
                               std::to_string(error) + "): " + description)
                                  .c_str());
}

bool s_GLFWInitialized = false;
int s_WindowCount = 0;

// Computes a letterboxed/pillarboxed viewport for the given framebuffer
// size and target aspect ratio, then applies it via glViewport.
// If targetAspect <= 0, the viewport fills the entire framebuffer.
void ApplyViewport(int fbWidth, int fbHeight, float targetAspect) {
  auto rect =
      Engine::Core::ComputeLetterboxedViewport(fbWidth, fbHeight, targetAspect);
  glViewport(rect.X, rect.Y, rect.Width, rect.Height);
}

} // namespace

namespace Engine::Platform {

Window::Window(const WindowProps &props) { Init(props); }

Window::~Window() { Shutdown(); }

void Window::Init(const WindowProps &props) {
  m_Data.Title = props.Title;
  m_Data.Width = props.Width;
  m_Data.Height = props.Height;
  m_Data.VSync = props.VSync;

  if (!s_GLFWInitialized) {
    int success = glfwInit();
    ENGINE_ASSERT(success, "Failed to initialize GLFW");
    glfwSetErrorCallback(GLFWErrorCallback);
    s_GLFWInitialized = true;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width),
                              static_cast<int>(m_Data.Height),
                              m_Data.Title.c_str(), nullptr, nullptr);
  ENGINE_ASSERT(m_Window != nullptr, "Failed to create GLFW window");
  ++s_WindowCount;

  glfwSetWindowUserPointer(m_Window, &m_Data);

  glfwMakeContextCurrent(m_Window);

  int gladVersion = gladLoadGL(glfwGetProcAddress);
  ENGINE_ASSERT(gladVersion != 0, "Failed to initialize GLAD");

  // Ensure the GL viewport matches the actual framebuffer size at
  // creation time (accounts for HiDPI displays where framebuffer size
  // may differ from the requested window size).
  int fbWidth, fbHeight;
  glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
  ApplyViewport(fbWidth, fbHeight, m_Data.TargetAspectRatio);

  Core::Logger::Info((std::string("Created window: ") + m_Data.Title + " (" +
                      std::to_string(m_Data.Width) + "x" +
                      std::to_string(m_Data.Height) + ")")
                         .c_str());
  Core::Logger::Info((std::string("OpenGL: ") +
                      reinterpret_cast<const char *>(glGetString(GL_VERSION)))
                         .c_str());

  SetVSync(m_Data.VSync);

  // --- GLFW callbacks: translate raw GLFW notifications into engine Events ---

  glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow *window, int width,
                                              int height) {
    auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    ApplyViewport(width, height, data.TargetAspectRatio);
  });

  glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width,
                                         int height) {
    auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    data.Width = static_cast<uint32_t>(width);
    data.Height = static_cast<uint32_t>(height);

    Events::WindowResizeEvent event(data.Width, data.Height);
    if (data.EventCallback) {
      data.EventCallback(event);
    }
  });

  glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
    auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));

    Events::WindowCloseEvent event;
    if (data.EventCallback) {
      data.EventCallback(event);
    }
  });

  glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode,
                                  int action, int mods) {
    auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    auto engineKey = static_cast<Input::Key>(key);

    switch (action) {
    case GLFW_PRESS: {
      Input::Input::SetKeyState(engineKey, true);
      Events::KeyPressedEvent event(engineKey, false);
      if (data.EventCallback)
        data.EventCallback(event);
      break;
    }
    case GLFW_RELEASE: {
      Input::Input::SetKeyState(engineKey, false);
      Events::KeyReleasedEvent event(engineKey);
      if (data.EventCallback)
        data.EventCallback(event);
      break;
    }
    case GLFW_REPEAT: {
      Events::KeyPressedEvent event(engineKey, true);
      if (data.EventCallback)
        data.EventCallback(event);
      break;
    }
    }
  });

  glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button,
                                          int action, int mods) {
    auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    auto engineButton = static_cast<Input::MouseButton>(button);

    if (action == GLFW_PRESS) {
      Input::Input::SetMouseButtonState(engineButton, true);
      Events::MouseButtonPressedEvent event(engineButton);
      if (data.EventCallback)
        data.EventCallback(event);
    } else if (action == GLFW_RELEASE) {
      Input::Input::SetMouseButtonState(engineButton, false);
      Events::MouseButtonReleasedEvent event(engineButton);
      if (data.EventCallback)
        data.EventCallback(event);
    }
  });

  glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos,
                                        double yPos) {
    auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    Input::Input::SetMousePosition(static_cast<float>(xPos),
                                   static_cast<float>(yPos));

    Events::MouseMovedEvent event(static_cast<float>(xPos),
                                  static_cast<float>(yPos));
    if (data.EventCallback)
      data.EventCallback(event);
  });

  glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset,
                                     double yOffset) {
    auto &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    Events::MouseScrolledEvent event(static_cast<float>(xOffset),
                                     static_cast<float>(yOffset));
    if (data.EventCallback)
      data.EventCallback(event);
  });
}

void Window::LockAspectRatio(float aspectRatio) {
  m_Data.TargetAspectRatio = aspectRatio;

  if (m_Window) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
    ApplyViewport(fbWidth, fbHeight, m_Data.TargetAspectRatio);
  }
}

void Window::Shutdown() {
  if (m_Window) {
    glfwDestroyWindow(m_Window);
    m_Window = nullptr;
    --s_WindowCount;
  }

  if (s_WindowCount == 0 && s_GLFWInitialized) {
    glfwTerminate();
    s_GLFWInitialized = false;
  }
}

void Window::OnUpdate() {
  glfwPollEvents();
  glfwSwapBuffers(m_Window);
}

bool Window::ShouldClose() const { return glfwWindowShouldClose(m_Window); }

void Window::SetVSync(bool enabled) {
  glfwSwapInterval(enabled ? 1 : 0);
  m_Data.VSync = enabled;
}

} // namespace Engine::Platform