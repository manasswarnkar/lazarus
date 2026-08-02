#include "Window.h"
#include "Core/Assert.h"
#include "Core/Logger.h"

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

// Called whenever the actual framebuffer (in pixels, not screen
// coordinates) changes size — the correct callback to resize the GL
// viewport from, since it accounts for HiDPI/Retina scaling that
// glfwSetWindowSizeCallback would not.
void FramebufferSizeCallback(GLFWwindow *window, int width, int height) {
  auto *data =
      static_cast<Engine::Platform::Window *>(glfwGetWindowUserPointer(window));
  (void)data; // width/height applied directly below; data reserved for
              // future use (e.g. dispatching a WindowResizeEvent)

  glViewport(0, 0, width, height);
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

  glfwSetWindowUserPointer(m_Window, this);

  glfwMakeContextCurrent(m_Window);

  int gladVersion = gladLoadGL(glfwGetProcAddress);
  ENGINE_ASSERT(gladVersion != 0, "Failed to initialize GLAD");

  // Ensure the GL viewport matches the actual framebuffer size at
  // creation time (accounts for HiDPI displays where framebuffer size
  // may differ from the requested window size).
  int fbWidth, fbHeight;
  glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
  glViewport(0, 0, fbWidth, fbHeight);

  glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

  glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width,
                                         int height) {
    auto *win = static_cast<Window *>(glfwGetWindowUserPointer(window));
    win->m_Data.Width = static_cast<uint32_t>(width);
    win->m_Data.Height = static_cast<uint32_t>(height);
  });

  Core::Logger::Info((std::string("Created window: ") + m_Data.Title + " (" +
                      std::to_string(m_Data.Width) + "x" +
                      std::to_string(m_Data.Height) + ")")
                         .c_str());
  Core::Logger::Info((std::string("OpenGL: ") +
                      reinterpret_cast<const char *>(glGetString(GL_VERSION)))
                         .c_str());

  SetVSync(m_Data.VSync);
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