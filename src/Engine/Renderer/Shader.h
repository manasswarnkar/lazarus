#pragma once

#include <cstdint>
#include <string>

namespace Engine::Renderer {

class Shader {
public:
  Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
  ~Shader();

  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  void Bind() const;
  void Unbind() const;

  void SetInt(const std::string &name, int value);
  void SetFloat4(const std::string &name, float v0, float v1, float v2,
                 float v3);

private:
  int GetUniformLocation(const std::string &name) const;
  uint32_t m_RendererID = 0;
};

} // namespace Engine::Renderer