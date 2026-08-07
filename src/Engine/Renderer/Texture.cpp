#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Core/Assert.h"
#include "Core/Logger.h"
#include "Texture.h"

#include <glad/gl.h>

namespace Engine::Renderer {

Texture2D::Texture2D(const std::string &path) : m_Path(path) {
  int width, height, channels;

  // OpenGL expects the first row of pixel data to be the bottom of the
  // image; most image formats store top-to-bottom. This flip corrects
  // for that so textures don't render upside-down.
  stbi_set_flip_vertically_on_load(1);

  stbi_uc *data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  ENGINE_ASSERT(data != nullptr, ("Failed to load texture: " + path).c_str());

  m_Width = static_cast<uint32_t>(width);
  m_Height = static_cast<uint32_t>(height);

  GLenum internalFormat = 0, dataFormat = 0;
  if (channels == 4) {
    internalFormat = GL_RGBA8;
    dataFormat = GL_RGBA;
  } else if (channels == 3) {
    internalFormat = GL_RGB8;
    dataFormat = GL_RGB;
  } else {
    stbi_image_free(data);
    ENGINE_ASSERT(false, ("Unsupported channel count (" +
                          std::to_string(channels) + ") in texture: " + path)
                             .c_str());
  }

  glGenTextures(1, &m_RendererID);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(internalFormat), width,
               height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);

  Core::Logger::Info(("Loaded texture: " + path + " (" + std::to_string(width) +
                      "x" + std::to_string(height) + ", " +
                      std::to_string(channels) + " channels)")
                         .c_str());
}

Texture2D::~Texture2D() { glDeleteTextures(1, &m_RendererID); }

void Texture2D::Bind(uint32_t slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

} // namespace Engine::Renderer