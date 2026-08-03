#include "VertexArray.h"
#include "Core/Assert.h"

#include <glad/gl.h>

namespace {
GLenum ShaderDataTypeToGLBaseType(Engine::Renderer::ShaderDataType type) {
  using Engine::Renderer::ShaderDataType;
  switch (type) {
  case ShaderDataType::Float:
  case ShaderDataType::Float2:
  case ShaderDataType::Float3:
  case ShaderDataType::Float4:
    return GL_FLOAT;
  case ShaderDataType::Int:
  case ShaderDataType::Int2:
  case ShaderDataType::Int3:
  case ShaderDataType::Int4:
    return GL_INT;
  case ShaderDataType::Bool:
    return GL_BOOL;
  case ShaderDataType::None:
    break;
  }
  ENGINE_ASSERT(false, "Unknown ShaderDataType");
  return 0;
}
} // namespace

namespace Engine::Renderer {
VertexArray::VertexArray() { glGenVertexArrays(1, &m_RendererID); }

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_RendererID); }

void VertexArray::Bind() const { glBindVertexArray(m_RendererID); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::AddVertexBuffer(
    const std::shared_ptr<VertexBuffer> &vertexBuffer) {
  ENGINE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(),
                "VertexBuffer has no layout set");

  glBindVertexArray(m_RendererID);
  vertexBuffer->Bind();

  const auto &layout = vertexBuffer->GetLayout();
  for (const auto &element : layout) {
    glEnableVertexAttribArray(m_VertexBufferIndex);
    glVertexAttribPointer(m_VertexBufferIndex,
                          static_cast<GLint>(element.GetComponentCount()),
                          ShaderDataTypeToGLBaseType(element.Type),
                          element.Normalized ? GL_TRUE : GL_FALSE,
                          static_cast<GLsizei>(layout.GetStride()),
                          reinterpret_cast<const void *>(element.Offset));
    ++m_VertexBufferIndex;
  }

  m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(
    const std::shared_ptr<IndexBuffer> &indexBuffer) {
  glBindVertexArray(m_RendererID);
  indexBuffer->Bind();
  m_IndexBuffer = indexBuffer;
}

} // namespace Engine::Renderer