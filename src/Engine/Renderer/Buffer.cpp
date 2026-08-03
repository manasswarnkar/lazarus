#include "Buffer.h"
#include "Core/Assert.h"

#include <glad/gl.h>

namespace Engine::Renderer {

uint32_t ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
  case ShaderDataType::Float:
    return 4;
  case ShaderDataType::Float2:
    return 4 * 2;
  case ShaderDataType::Float3:
    return 4 * 3;
  case ShaderDataType::Float4:
    return 4 * 4;
  case ShaderDataType::Int:
    return 4;
  case ShaderDataType::Int2:
    return 4 * 2;
  case ShaderDataType::Int3:
    return 4 * 3;
  case ShaderDataType::Int4:
    return 4 * 4;
  case ShaderDataType::Bool:
    return 1;
  case ShaderDataType::None:
    break;
  }
  ENGINE_ASSERT(false, "Unknown ShaderDataType");
  return 0;
}

uint32_t BufferElement::GetComponentCount() const {
  switch (Type) {
  case ShaderDataType::Float:
    return 1;
  case ShaderDataType::Float2:
    return 2;
  case ShaderDataType::Float3:
    return 3;
  case ShaderDataType::Float4:
    return 4;
  case ShaderDataType::Int:
    return 1;
  case ShaderDataType::Int2:
    return 2;
  case ShaderDataType::Int3:
    return 3;
  case ShaderDataType::Int4:
    return 4;
  case ShaderDataType::Bool:
    return 1;
  case ShaderDataType::None:
    break;
  }
  ENGINE_ASSERT(false, "Unknown ShaderDataType");
  return 0;
}

// ---------- VertexBuffer ----------

VertexBuffer::VertexBuffer(const float *vertices, uint32_t size) {
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void VertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); }

void VertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// ---------- IndexBuffer ----------

IndexBuffer::IndexBuffer(const uint32_t *indices, uint32_t count)
    : m_Count(count) {
  glGenBuffers(1, &m_RendererID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_RendererID); }

void IndexBuffer::Bind() const {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

} // namespace Engine::Renderer