#pragma once

#include "VertexArray.h"
#include <memory>

namespace Engine::Renderer {

class RenderCommand {
public:
  static void SetClearColor(float r, float g, float b, float a);
  static void Clear();
  static void DrawIndexed(const std::shared_ptr<VertexArray> &vertexArray);
};

} // namespace Engine::Renderer