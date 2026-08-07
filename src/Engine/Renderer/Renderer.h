#pragma once

#include "Material.h"
#include "OrthographicCamera.h"
#include "VertexArray.h"
#include <memory>

namespace Engine::Renderer {

class Renderer {
public:
  static void BeginScene(const OrthographicCamera &camera);
  static void EndScene();

  static void Submit(const std::shared_ptr<Material> &material,
                     const std::shared_ptr<VertexArray> &vertexArray);

private:
  struct SceneData {
    glm::mat4 ViewProjectionMatrix;
  };
  static SceneData *s_SceneData;
};

} // namespace Engine::Renderer