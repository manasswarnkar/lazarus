#include "Renderer/OrthographicCamera.h"
#include <glm/gtc/epsilon.hpp>
#include <gtest/gtest.h>

using namespace Engine::Renderer;

namespace {
constexpr float kEpsilon = 0.0001f;

bool MatricesEqual(const glm::mat4 &a, const glm::mat4 &b) {
  for (int col = 0; col < 4; ++col) {
    for (int row = 0; row < 4; ++row) {
      if (std::abs(a[col][row] - b[col][row]) > kEpsilon) {
        return false;
      }
    }
  }
  return true;
}
} // namespace

TEST(OrthographicCameraTest, InitialPositionIsOrigin) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  EXPECT_EQ(camera.GetPosition(), glm::vec3(0.0f, 0.0f, 0.0f));
}

TEST(OrthographicCameraTest, InitialRotationIsZero) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  EXPECT_FLOAT_EQ(camera.GetRotation(), 0.0f);
}

TEST(OrthographicCameraTest, InitialViewMatrixIsIdentity) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  EXPECT_TRUE(MatricesEqual(camera.GetViewMatrix(), glm::mat4(1.0f)));
}

TEST(OrthographicCameraTest, SetPositionUpdatesGetPosition) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  camera.SetPosition({1.0f, 2.0f, 0.0f});

  EXPECT_EQ(camera.GetPosition(), glm::vec3(1.0f, 2.0f, 0.0f));
}

TEST(OrthographicCameraTest, SetPositionChangesViewMatrix) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  glm::mat4 initialView = camera.GetViewMatrix();

  camera.SetPosition({1.0f, 0.0f, 0.0f});

  EXPECT_FALSE(MatricesEqual(camera.GetViewMatrix(), initialView));
}

TEST(OrthographicCameraTest, ViewMatrixIsInverseOfCameraTransform) {
  // Moving the camera to (1, 0, 0) should translate the view matrix by
  // (-1, 0, 0) — the view matrix is the inverse of the camera's world
  // transform (world moves opposite to camera movement).
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  camera.SetPosition({1.0f, 0.0f, 0.0f});

  glm::vec4 worldOrigin(0.0f, 0.0f, 0.0f, 1.0f);
  glm::vec4 viewSpacePoint = camera.GetViewMatrix() * worldOrigin;

  EXPECT_NEAR(viewSpacePoint.x, -1.0f, kEpsilon);
}

TEST(OrthographicCameraTest, SetRotationUpdatesGetRotation) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  camera.SetRotation(45.0f);

  EXPECT_FLOAT_EQ(camera.GetRotation(), 45.0f);
}

TEST(OrthographicCameraTest, SetRotationChangesViewMatrix) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  glm::mat4 initialView = camera.GetViewMatrix();

  camera.SetRotation(90.0f);

  EXPECT_FALSE(MatricesEqual(camera.GetViewMatrix(), initialView));
}

TEST(OrthographicCameraTest, ViewProjectionIsProductOfViewAndProjection) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  camera.SetPosition({0.5f, 0.5f, 0.0f});

  glm::mat4 expected = camera.GetProjectionMatrix() * camera.GetViewMatrix();
  EXPECT_TRUE(MatricesEqual(camera.GetViewProjectionMatrix(), expected));
}

TEST(OrthographicCameraTest, SetProjectionChangesProjectionMatrix) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  glm::mat4 initialProjection = camera.GetProjectionMatrix();

  camera.SetProjection(-2.0f, 2.0f, -1.0f, 1.0f);

  EXPECT_FALSE(MatricesEqual(camera.GetProjectionMatrix(), initialProjection));
}

TEST(OrthographicCameraTest, SetProjectionDoesNotAffectViewMatrix) {
  OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);
  camera.SetPosition({1.0f, 1.0f, 0.0f});
  glm::mat4 viewBefore = camera.GetViewMatrix();

  camera.SetProjection(-2.0f, 2.0f, -1.0f, 1.0f);

  EXPECT_TRUE(MatricesEqual(camera.GetViewMatrix(), viewBefore));
}