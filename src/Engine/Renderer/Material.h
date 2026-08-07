#pragma once

#include "Shader.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <memory>
#include <string>
#include <unordered_map>

namespace Engine::Renderer {

// Bundles a Shader with the texture(s) and uniform values it needs for
// a single draw. Keeps Main.cpp / calling code from manually managing
// shader->SetInt / texture->Bind calls per object.
class Material {
public:
  explicit Material(std::shared_ptr<Shader> shader);

  void SetTexture(const std::string &uniformName,
                  std::shared_ptr<Texture2D> texture, uint32_t slot = 0);
  void SetTintColor(const glm::vec4 &color) { m_TintColor = color; }

  // Binds the shader, binds all textures to their slots, and uploads
  // all uniform values. Call before submitting a draw.
  void Bind() const;

  std::shared_ptr<Shader> GetShader() const { return m_Shader; }

private:
  struct TextureBinding {
    std::shared_ptr<Texture2D> Texture;
    uint32_t Slot;
  };

  std::shared_ptr<Shader> m_Shader;
  std::unordered_map<std::string, TextureBinding> m_Textures;
  glm::vec4 m_TintColor = {1.0f, 1.0f, 1.0f, 1.0f};
};

} // namespace Engine::Renderer