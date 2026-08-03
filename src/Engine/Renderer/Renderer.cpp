#include "Renderer.h"
#include "RenderCommand.h"

namespace Engine::Renderer {

Renderer::SceneData *Renderer::s_SceneData = new SceneData();

void Renderer::BeginScene(const OrthographicCamera &camera) {
  s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<Shader> &shader,
                      const std::shared_ptr<VertexArray> &vertexArray) {
  shader->Bind();
  shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
  RenderCommand::DrawIndexed(vertexArray);
}

} // namespace Engine::Renderer