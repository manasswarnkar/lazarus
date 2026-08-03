#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/Event.h"
#include "OrthographicCamera.h"

namespace Engine::Renderer {

// Owns an OrthographicCamera and updates it each frame based on Input
// state (WASD/arrow movement, Q/E rotation). Also keeps the camera's
// aspect ratio correct in response to WindowResizeEvent.
class OrthographicCameraController {
public:
  explicit OrthographicCameraController(float aspectRatio,
                                        bool enableRotation = false);

  void OnUpdate(float deltaTime);
  void OnEvent(Events::Event &e);

  OrthographicCamera &GetCamera() { return m_Camera; }
  const OrthographicCamera &GetCamera() const { return m_Camera; }

  float GetZoomLevel() const { return m_ZoomLevel; }
  void SetZoomLevel(float level) {
    m_ZoomLevel = level;
    RecalculateProjection();
  }

private:
  bool OnWindowResized(Events::WindowResizeEvent &e);
  void RecalculateProjection();

  float m_AspectRatio;
  float m_ZoomLevel = 1.0f;
  OrthographicCamera m_Camera;

  bool m_RotationEnabled;

  glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
  float m_CameraRotation = 0.0f;
  float m_CameraTranslationSpeed = 2.0f; // units per second
  float m_CameraRotationSpeed = 90.0f;   // degrees per second
};

} // namespace Engine::Renderer