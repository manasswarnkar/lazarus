#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "Platform/Window.h"
#include "Renderer/Buffer.h"
#include "Renderer/OrthographicCameraController.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/VertexArray.h"

using namespace Engine::Core;
using namespace Engine::Platform;
using namespace Engine::Events;
using namespace Engine::Renderer;

int main() {
  Logger::Info("Engine starting...");

  bool running = true;

  Window window({"Lazarus Engine", 1280, 720, true});
  window.LockAspectRatio(1280.0f / 720.0f);
  OrthographicCameraController cameraController(1280.0f / 720.0f, true);

  window.SetEventCallback([&](Event &e) {
    EventDispatcher dispatcher(e);

    dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent &) {
      Logger::Info("Window close requested.");
      running = false;
      return true;

      cameraController.OnEvent(e);
    });

    // dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent &re) {
    //   Logger::Info(re.ToString().c_str());
    //   return true;
    // });
  });

  float vertices[] = {
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,
      0.5f,  0.5f,  0.0f, 1.0f, 1.0f, -0.5f, 0.5f,  0.0f, 0.0f, 1.0f,
  };
  uint32_t indices[] = {0, 1, 2, 2, 3, 0};

  auto vertexArray = std::make_shared<VertexArray>();

  auto vertexBuffer =
      std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
  vertexBuffer->SetLayout({
      {ShaderDataType::Float3, "a_Position"},
      {ShaderDataType::Float2, "a_TexCoord"},
  });
  vertexArray->AddVertexBuffer(vertexBuffer);

  auto indexBuffer = std::make_shared<IndexBuffer>(indices, 6);
  vertexArray->SetIndexBuffer(indexBuffer);

  auto shader = Shader::CreateFromFiles("assets/shaders/Texture.vert",
                                        "assets/shaders/Texture.frag");

  auto texture = std::make_shared<Texture2D>("assets/textures/test.png");

  auto material = std::make_shared<Material>(shader);
  material->SetTexture("u_Texture", texture, 0);
  material->SetTintColor(
      {1.0f, 0.5f, 0.5f, 1.0f}); // try {1, 0.5, 0.5, 1} for a red tint

  while (running && !window.ShouldClose()) {
    Time::Update();
    float deltaTime = static_cast<float>(Time::DeltaTime());

    cameraController.OnUpdate(deltaTime);

    RenderCommand::SetClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    RenderCommand::Clear();

    texture->Bind(0);

    Renderer::BeginScene(cameraController.GetCamera());
    Renderer::Submit(material, vertexArray);
    Renderer::EndScene();

    window.OnUpdate();
  }

  Logger::Info("Engine shutting down.");
  return 0;
}