#include "Material.h"
#include "Core/Assert.h"

namespace Engine::Renderer {

Material::Material(std::shared_ptr<Shader> shader)
    : m_Shader(std::move(shader)) {
  ENGINE_ASSERT(m_Shader != nullptr, "Material requires a valid Shader");
}

void Material::SetTexture(const std::string &uniformName,
                          std::shared_ptr<Texture2D> texture, uint32_t slot) {
  m_Textures[uniformName] = {std::move(texture), slot};
}

void Material::Bind() const {
  m_Shader->Bind();

  for (const auto &[uniformName, binding] : m_Textures) {
    binding.Texture->Bind(binding.Slot);
    m_Shader->SetInt(uniformName, static_cast<int>(binding.Slot));
  }

  m_Shader->SetFloat4("u_TintColor", m_TintColor.r, m_TintColor.g,
                      m_TintColor.b, m_TintColor.a);
}

} // namespace Engine::Renderer