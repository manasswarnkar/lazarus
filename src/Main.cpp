#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Platform/Window.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

#include "glad/gl.h"

using namespace Engine::Core;
using namespace Engine::Platform;
using namespace Engine::Events;
using namespace Engine::Renderer;

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

  float vertices[] = {
      -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
  };
  uint32_t indices[] = {0, 1, 2};

  auto vertexArray = std::make_shared<VertexArray>();

  auto vertexBuffer =
      std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
  vertexBuffer->SetLayout({{ShaderDataType::Float3, "a_Position"}});
  vertexArray->AddVertexBuffer(vertexBuffer);

  auto indexBuffer = std::make_shared<IndexBuffer>(indices, 3);
  vertexArray->SetIndexBuffer(indexBuffer);

  std::string vertexSrc = R"(
    #version 450 core
    layout(location = 0) in vec3 a_Position;
    void main() {
      gl_Position = vec4(a_Position, 1.0);
    }
  )";

  std::string fragmentSrc = R"(
    #version 450 core
    layout(location = 0) out vec4 color;
    void main() {
      color = vec4(0.9, 0.4, 0.2, 1.0);
    }
  )";

  Shader shader(vertexSrc, fragmentSrc);

  while (running && !window.ShouldClose()) {
    Time::Update();

    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.Bind();
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT,
                   nullptr);

    window.OnUpdate();
  }

  Logger::Info("Engine shutting down.");
  return 0;
}