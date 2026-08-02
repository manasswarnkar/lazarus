#pragma once

#include "KeyCodes.h"
#include "MouseCodes.h"
#include <array>
#include <utility>

namespace Engine::Input {

// Static, polling-friendly input state. State is populated by Window's
// GLFW callbacks (see Window.cpp) — Input itself has no GLFW dependency.
class Input {
public:
  static bool IsKeyPressed(Key key);
  static bool IsMouseButtonPressed(MouseButton button);

  static std::pair<float, float> GetMousePosition();
  static float GetMouseX();
  static float GetMouseY();

  // --- Called only by Window's callbacks to update cached state ---
  static void SetKeyState(Key key, bool pressed);
  static void SetMouseButtonState(MouseButton button, bool pressed);
  static void SetMousePosition(float x, float y);

private:
  static constexpr std::size_t s_MaxKeyCode = 350;
  static constexpr std::size_t s_MaxMouseButton = 8;

  static std::array<bool, s_MaxKeyCode> s_Keys;
  static std::array<bool, s_MaxMouseButton> s_MouseButtons;
  static float s_MouseX, s_MouseY;
};

} // namespace Engine::Input