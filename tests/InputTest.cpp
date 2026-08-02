#include "Input/Input.h"
#include <gtest/gtest.h>

using namespace Engine::Input;

TEST(InputTest, KeyIsNotPressedByDefault) {
  EXPECT_FALSE(Input::IsKeyPressed(Key::G));
}

TEST(InputTest, SetKeyStatePressedIsReflected) {
  Input::SetKeyState(Key::W, true);
  EXPECT_TRUE(Input::IsKeyPressed(Key::W));

  Input::SetKeyState(Key::W, false); // reset for other tests
}

TEST(InputTest, SetKeyStateReleasedIsReflected) {
  Input::SetKeyState(Key::A, true);
  ASSERT_TRUE(Input::IsKeyPressed(Key::A));

  Input::SetKeyState(Key::A, false);
  EXPECT_FALSE(Input::IsKeyPressed(Key::A));
}

TEST(InputTest, MouseButtonIsNotPressedByDefault) {
  EXPECT_FALSE(Input::IsMouseButtonPressed(MouseButton::Middle));
}

TEST(InputTest, SetMouseButtonStateIsReflected) {
  Input::SetMouseButtonState(MouseButton::Left, true);
  EXPECT_TRUE(Input::IsMouseButtonPressed(MouseButton::Left));

  Input::SetMouseButtonState(MouseButton::Left, false); // reset
}

TEST(InputTest, MousePositionDefaultsToZero) {
  auto [x, y] = Input::GetMousePosition();
  EXPECT_FLOAT_EQ(x, 0.0f);
  EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(InputTest, SetMousePositionUpdatesGetters) {
  Input::SetMousePosition(123.5f, 456.25f);

  EXPECT_FLOAT_EQ(Input::GetMouseX(), 123.5f);
  EXPECT_FLOAT_EQ(Input::GetMouseY(), 456.25f);

  auto [x, y] = Input::GetMousePosition();
  EXPECT_FLOAT_EQ(x, 123.5f);
  EXPECT_FLOAT_EQ(y, 456.25f);

  Input::SetMousePosition(0.0f, 0.0f); // reset
}

TEST(InputTest, MultipleKeysTrackedIndependently) {
  Input::SetKeyState(Key::S, true);
  Input::SetKeyState(Key::D, true);

  EXPECT_TRUE(Input::IsKeyPressed(Key::S));
  EXPECT_TRUE(Input::IsKeyPressed(Key::D));
  EXPECT_FALSE(Input::IsKeyPressed(Key::W));

  Input::SetKeyState(Key::S, false);
  Input::SetKeyState(Key::D, false);
}