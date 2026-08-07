#pragma once

#include <cstdint>
#include <string>

namespace Engine::Renderer {

class Texture2D {
public:
  // Loads an image from disk (PNG, JPG, etc. via stb_image) and uploads
  // it to the GPU. Asserts on failure — a missing/corrupt texture file
  // is treated as fatal, same reasoning as Shader::CreateFromFiles.
  explicit Texture2D(const std::string &path);
  ~Texture2D();

  Texture2D(const Texture2D &) = delete;
  Texture2D &operator=(const Texture2D &) = delete;

  uint32_t GetWidth() const { return m_Width; }
  uint32_t GetHeight() const { return m_Height; }

  // Binds to the given texture unit slot (default 0), matching
  // GL_TEXTURE0 + slot.
  void Bind(uint32_t slot = 0) const;

private:
  uint32_t m_RendererID = 0;
  uint32_t m_Width = 0, m_Height = 0;
  std::string m_Path;
};

} // namespace Engine::Renderer