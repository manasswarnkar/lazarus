#pragma once

#include "Buffer.h"
#include <memory>
#include <vector>

namespace Engine::Renderer {

// Binds together one or more VertexBuffers (with their layouts) and an
// IndexBuffer into a single GPU-side "recipe" for how to draw a mesh —
// this is what you bind before issuing a draw call.
class VertexArray {
public:
  VertexArray();
  ~VertexArray();

  VertexArray(const VertexArray &) = delete;
  VertexArray &operator=(const VertexArray &) = delete;

  void Bind() const;
  void Unbind() const;

  void AddVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer);
  void SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer);

  const std::shared_ptr<IndexBuffer> &GetIndexBuffer() const {
    return m_IndexBuffer;
  }

private:
  uint32_t m_RendererID = 0;
  uint32_t m_VertexBufferIndex = 0;
  std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
  std::shared_ptr<IndexBuffer> m_IndexBuffer;
};


} // namespace Engine::Renderer