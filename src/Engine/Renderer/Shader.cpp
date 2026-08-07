#include "Shader.h"
#include "Core/Assert.h"
#include "Core/FileSystem.h"
#include "Core/Logger.h"

#include <glad/gl.h>
#include <vector>

namespace Engine::Renderer {

namespace {
uint32_t CompileShader(GLenum type, const std::string &source) {
  uint32_t shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  int isCompiled = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE) {
    int maxLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<char> infoLog(maxLength);
    glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

    Core::Logger::Error(
        (std::string("Shader compilation failed: ") + infoLog.data()).c_str());

    glDeleteShader(shader);
    ENGINE_ASSERT(false, "Shader compilation failed");
    return 0;
  }

  return shader;
}
} // namespace

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc) {
  uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
  uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);

  m_RendererID = glCreateProgram();
  glAttachShader(m_RendererID, vertexShader);
  glAttachShader(m_RendererID, fragmentShader);
  glLinkProgram(m_RendererID);

  int isLinked = 0;
  glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
  if (isLinked == GL_FALSE) {
    int maxLength = 0;
    glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<char> infoLog(maxLength);
    glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, infoLog.data());

    Core::Logger::Error(
        (std::string("Shader linking failed: ") + infoLog.data()).c_str());

    glDeleteProgram(m_RendererID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    ENGINE_ASSERT(false, "Shader linking failed");
    return;
  }

  glDetachShader(m_RendererID, vertexShader);
  glDetachShader(m_RendererID, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

Shader::~Shader() { glDeleteProgram(m_RendererID); }

void Shader::Bind() const { glUseProgram(m_RendererID); }
void Shader::Unbind() const { glUseProgram(0); }

int Shader::GetUniformLocation(const std::string &name) const {
  int location = glGetUniformLocation(m_RendererID, name.c_str());
  ENGINE_ASSERT(location != -1,
                ("Uniform '" + name + "' not found in shader").c_str());
  return location;
}

void Shader::SetInt(const std::string &name, int value) {
  glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat4(const std::string &name, float v0, float v1, float v2,
                       float v3) {
  glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &matrix) {
  glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

std::shared_ptr<Shader>
Shader::CreateFromFiles(const std::string &vertexPath,
                        const std::string &fragmentPath) {
  auto vertexSrc = Core::FileSystem::ReadFileToString(vertexPath);
  ENGINE_ASSERT(vertexSrc.has_value(),
                ("Failed to read vertex shader: " + vertexPath).c_str());

  auto fragmentSrc = Core::FileSystem::ReadFileToString(fragmentPath);
  ENGINE_ASSERT(fragmentSrc.has_value(),
                ("Failed to read fragment shader: " + fragmentPath).c_str());

  return std::make_shared<Shader>(*vertexSrc, *fragmentSrc);
}

} // namespace Engine::Renderer