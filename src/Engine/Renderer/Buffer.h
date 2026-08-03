#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace Engine::Renderer {

enum class ShaderDataType {
  None = 0,
  Float,
  Float2,
  Float3,
  Float4,
  Int,
  Int2,
  Int3,
  Int4,
  Bool
};

// Size in bytes of a given ShaderDataType.
uint32_t ShaderDataTypeSize(ShaderDataType type);

struct BufferElement {
  std::string Name;
  ShaderDataType Type;
  uint32_t Size;
  size_t Offset;
  bool Normalized;

  BufferElement(ShaderDataType type, const std::string &name,
                bool normalized = false)
      : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0),
        Normalized(normalized) {}

  uint32_t GetComponentCount() const;
};

// Describes the layout of a single vertex — an ordered list of
// BufferElements, with strides/offsets computed automatically.
class BufferLayout {
public:
  BufferLayout() = default;
  BufferLayout(std::initializer_list<BufferElement> elements)
      : m_Elements(elements) {
    CalculateOffsetsAndStride();
  }

  uint32_t GetStride() const { return m_Stride; }
  const std::vector<BufferElement> &GetElements() const { return m_Elements; }

  std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
  std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
  std::vector<BufferElement>::const_iterator begin() const {
    return m_Elements.begin();
  }
  std::vector<BufferElement>::const_iterator end() const {
    return m_Elements.end();
  }

private:
  void CalculateOffsetsAndStride() {
    size_t offset = 0;
    m_Stride = 0;
    for (auto &element : m_Elements) {
      element.Offset = offset;
      offset += element.Size;
      m_Stride += element.Size;
    }
  }

  std::vector<BufferElement> m_Elements;
  uint32_t m_Stride = 0;
};

class VertexBuffer {
public:
  VertexBuffer(const float *vertices, uint32_t size);
  ~VertexBuffer();

  VertexBuffer(const VertexBuffer &) = delete;
  VertexBuffer &operator=(const VertexBuffer &) = delete;

  void Bind() const;
  void Unbind() const;

  const BufferLayout &GetLayout() const { return m_Layout; }
  void SetLayout(const BufferLayout &layout) { m_Layout = layout; }

private:
  uint32_t m_RendererID = 0;
  BufferLayout m_Layout;
};

class IndexBuffer {
public:
  IndexBuffer(const uint32_t *indices, uint32_t count);
  ~IndexBuffer();

  IndexBuffer(const IndexBuffer &) = delete;
  IndexBuffer &operator=(const IndexBuffer &) = delete;

  void Bind() const;
  void Unbind() const;

  uint32_t GetCount() const { return m_Count; }

private:
  uint32_t m_RendererID = 0;
  uint32_t m_Count;
};

} // namespace Engine::Renderer