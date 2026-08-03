#include "OrthographicCameraController.h"
#include "Input/Input.h"
#include "Input/KeyCodes.h"

namespace Engine::Renderer {

OrthographicCameraController::OrthographicCameraController(float aspectRatio,
                                                           bool enableRotation)
    : m_AspectRatio(aspectRatio),
      m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel,
               -m_ZoomLevel, m_ZoomLevel),
      m_RotationEnabled(enableRotation) {}

void OrthographicCameraController::OnUpdate(float deltaTime) {
  using Engine::Input::Input;
  using Engine::Input::Key;

  if (Input::IsKeyPressed(Key::A)) {
    m_CameraPosition.x -= m_CameraTranslationSpeed * deltaTime;
  } else if (Input::IsKeyPressed(Key::D)) {
    m_CameraPosition.x += m_CameraTranslationSpeed * deltaTime;
  }

  if (Input::IsKeyPressed(Key::W)) {
    m_CameraPosition.y += m_CameraTranslationSpeed * deltaTime;
  } else if (Input::IsKeyPressed(Key::S)) {
    m_CameraPosition.y -= m_CameraTranslationSpeed * deltaTime;
  }

  if (m_RotationEnabled) {
    if (Input::IsKeyPressed(Key::Q)) {
      m_CameraRotation += m_CameraRotationSpeed * deltaTime;
    } else if (Input::IsKeyPressed(Key::E)) {
      m_CameraRotation -= m_CameraRotationSpeed * deltaTime;
    }
    m_Camera.SetRotation(m_CameraRotation);
  }

  m_Camera.SetPosition(m_CameraPosition);
}

void OrthographicCameraController::OnEvent(Events::Event &e) {
  Events::EventDispatcher dispatcher(e);
  dispatcher.Dispatch<Events::WindowResizeEvent>(
      [this](Events::WindowResizeEvent &re) { return OnWindowResized(re); });
}

bool OrthographicCameraController::OnWindowResized(
    Events::WindowResizeEvent &e) {
  m_AspectRatio =
      static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
  RecalculateProjection();
  return false; // don't consume — other systems may also want resize events
}

void OrthographicCameraController::RecalculateProjection() {
  m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel,
                         m_AspectRatio * m_ZoomLevel, -m_ZoomLevel,
                         m_ZoomLevel);
}

} // namespace Engine::Renderer