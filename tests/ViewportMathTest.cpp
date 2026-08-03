#include "Core/ViewportMath.h"
#include <gtest/gtest.h>

using namespace Engine::Core;

TEST(ViewportMathTest, DisabledAspectFillsEntireFramebuffer) {
  auto rect = ComputeLetterboxedViewport(1920, 1080, 0.0f);

  EXPECT_EQ(rect.X, 0);
  EXPECT_EQ(rect.Y, 0);
  EXPECT_EQ(rect.Width, 1920);
  EXPECT_EQ(rect.Height, 1080);
}

TEST(ViewportMathTest, MatchingAspectFillsEntireFramebuffer) {
  // 1280x720 is exactly 16:9 — target matches exactly, no bars.
  float targetAspect = 1280.0f / 720.0f;
  auto rect = ComputeLetterboxedViewport(1280, 720, targetAspect);

  EXPECT_EQ(rect.X, 0);
  EXPECT_EQ(rect.Y, 0);
  EXPECT_EQ(rect.Width, 1280);
  EXPECT_EQ(rect.Height, 720);
}

TEST(ViewportMathTest, WiderWindowProducesPillarbox) {
  // Target 16:9, window is 21:9 (ultrawide) — bars on left/right.
  float targetAspect = 16.0f / 9.0f;
  auto rect = ComputeLetterboxedViewport(2560, 1080, targetAspect);

  EXPECT_EQ(rect.Height, 1080); // full height used
  EXPECT_LT(rect.Width, 2560);  // narrower than framebuffer
  EXPECT_GT(rect.X, 0);         // offset from left, centered
}

TEST(ViewportMathTest, TallerWindowProducesLetterbox) {
  // Target 16:9, window is nearly square — bars on top/bottom.
  float targetAspect = 16.0f / 9.0f;
  auto rect = ComputeLetterboxedViewport(1000, 1000, targetAspect);

  EXPECT_EQ(rect.Width, 1000); // full width used
  EXPECT_LT(rect.Height, 1000);
  EXPECT_GT(rect.Y, 0);
}

TEST(ViewportMathTest, PillarboxIsHorizontallyCentered) {
  float targetAspect = 1.0f; // square target
  auto rect = ComputeLetterboxedViewport(2000, 1000, targetAspect);

  // Square viewport (1000x1000) inside a 2000x1000 framebuffer should
  // be centered: 500px bar on each side.
  EXPECT_EQ(rect.Width, 1000);
  EXPECT_EQ(rect.Height, 1000);
  EXPECT_EQ(rect.X, 500);
  EXPECT_EQ(rect.Y, 0);
}

TEST(ViewportMathTest, LetterboxIsVerticallyCentered) {
  float targetAspect = 1.0f; // square target
  auto rect = ComputeLetterboxedViewport(1000, 2000, targetAspect);

  EXPECT_EQ(rect.Width, 1000);
  EXPECT_EQ(rect.Height, 1000);
  EXPECT_EQ(rect.X, 0);
  EXPECT_EQ(rect.Y, 500);
}

TEST(ViewportMathTest, ZeroFramebufferSizeReturnsZeroedRect) {
  auto rect = ComputeLetterboxedViewport(0, 0, 16.0f / 9.0f);

  EXPECT_EQ(rect.Width, 0);
  EXPECT_EQ(rect.Height, 0);
}

TEST(ViewportMathTest, NegativeAspectDisablesLocking) {
  auto rect = ComputeLetterboxedViewport(1920, 1080, -1.0f);

  EXPECT_EQ(rect.Width, 1920);
  EXPECT_EQ(rect.Height, 1080);
}