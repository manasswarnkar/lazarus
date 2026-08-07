#include "Core/Random.h"
#include <gtest/gtest.h>

using namespace Engine::Core;

TEST(RandomTest, FloatIsWithinRange) {
  for (int i = 0; i < 1000; ++i) {
    float value = Random::Float(0.0f, 1.0f);
    EXPECT_GE(value, 0.0f);
    EXPECT_LT(value, 1.0f);
  }
}

TEST(RandomTest, FloatRespectsCustomRange) {
  for (int i = 0; i < 1000; ++i) {
    float value = Random::Float(-10.0f, -5.0f);
    EXPECT_GE(value, -10.0f);
    EXPECT_LT(value, -5.0f);
  }
}

TEST(RandomTest, IntIsWithinInclusiveRange) {
  for (int i = 0; i < 1000; ++i) {
    int value = Random::Int(1, 6); // dice roll
    EXPECT_GE(value, 1);
    EXPECT_LE(value, 6);
  }
}

TEST(RandomTest, IntCanReturnBothEndpoints) {
  // Not a strict guarantee in any single run, but over enough rolls
  // of a small range, both 1 and 6 should appear.
  bool sawMin = false, sawMax = false;

  for (int i = 0; i < 1000; ++i) {
    int value = Random::Int(1, 6);
    if (value == 1)
      sawMin = true;
    if (value == 6)
      sawMax = true;
  }

  EXPECT_TRUE(sawMin);
  EXPECT_TRUE(sawMax);
}

TEST(RandomTest, BoolWithZeroProbabilityIsAlwaysFalse) {
  for (int i = 0; i < 100; ++i) {
    EXPECT_FALSE(Random::Bool(0.0f));
  }
}

TEST(RandomTest, BoolWithFullProbabilityIsAlwaysTrue) {
  for (int i = 0; i < 100; ++i) {
    EXPECT_TRUE(Random::Bool(1.0f));
  }
}

TEST(RandomTest, SeedProducesReproducibleSequence) {
  Random::Seed(42);
  float a1 = Random::Float();
  int b1 = Random::Int(0, 100);

  Random::Seed(42); // reseed identically
  float a2 = Random::Float();
  int b2 = Random::Int(0, 100);

  EXPECT_FLOAT_EQ(a1, a2);
  EXPECT_EQ(b1, b2);
}

TEST(RandomTest, DifferentSeedsProduceDifferentSequences) {
  Random::Seed(1);
  float a = Random::Float();

  Random::Seed(2);
  float b = Random::Float();

  EXPECT_NE(a, b);
}