#include "Input.h"
#include "Core/Assert.h"

namespace Engine::Input {

std::array<bool, Input::s_MaxKeyCode> Input::s_Keys = {};
std::array<bool, Input::s_MaxMouseButton> Input::s_MouseButtons = {};
float Input::s_MouseX = 0.0f;
float Input::s_MouseY = 0.0f;

bool Input::IsKeyPressed(Key key) {
  auto index = static_cast<size_t>(key);
  ENGINE_ASSERT(index < s_MaxKeyCode, "Key code out of range");
  return s_Keys[index];
}

bool Input::IsMouseButtonPressed(MouseButton button) {
  auto index = static_cast<size_t>(button);
  ENGINE_ASSERT(index < s_MaxMouseButton, "Mouse button code out of range");
  return s_MouseButtons[index];
}

std::pair<float, float> Input::GetMousePosition() {
  return {s_MouseX, s_MouseY};
}

float Input::GetMouseX() { return s_MouseX; }
float Input::GetMouseY() { return s_MouseY; }

void Input::SetKeyState(Key key, bool pressed) {
  auto index = static_cast<size_t>(key);
  ENGINE_ASSERT(index < s_MaxKeyCode, "Key code out of range");
  s_Keys[index] = pressed;
}

void Input::SetMouseButtonState(MouseButton button, bool pressed) {
  auto index = static_cast<size_t>(button);
  ENGINE_ASSERT(index < s_MaxMouseButton, "Mouse button code out of range");
  s_MouseButtons[index] = pressed;
}

void Input::SetMousePosition(float x, float y) {
  s_MouseX = x;
  s_MouseY = y;
}

} // namespace Engine::Input