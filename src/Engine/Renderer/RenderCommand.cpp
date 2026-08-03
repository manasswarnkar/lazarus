#include "RenderCommand.h"
#include <glad/gl.h>

namespace Engine::Renderer {

void RenderCommand::SetClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void RenderCommand::Clear() { glClear(GL_COLOR_BUFFER_BIT); }

void RenderCommand::DrawIndexed(
    const std::shared_ptr<VertexArray> &vertexArray) {
  vertexArray->Bind();
  glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(),
                 GL_UNSIGNED_INT, nullptr);
}

} // namespace Engine::Renderer