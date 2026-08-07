#include "Random.h"
#include "Assert.h"

#include <random>

namespace Engine::Core {

namespace {
std::mt19937 s_Engine;
bool s_Initialized = false;
} // namespace

void Random::EnsureInitialized() {
  if (!s_Initialized) {
    std::random_device randomDevice;
    s_Engine.seed(randomDevice());
    s_Initialized = true;
  }
}

void Random::Seed(uint32_t seed) {
  s_Engine.seed(seed);
  s_Initialized = true;
}

float Random::Float(float min, float max) {
  ENGINE_ASSERT(min <= max, "Random::Float: min must be <= max");
  EnsureInitialized();

  std::uniform_real_distribution<float> distribution(min, max);
  return distribution(s_Engine);
}

int Random::Int(int min, int max) {
  ENGINE_ASSERT(min <= max, "Random::Int: min must be <= max");
  EnsureInitialized();

  std::uniform_int_distribution<int> distribution(min, max);
  return distribution(s_Engine);
}

bool Random::Bool(float probability) {
  ENGINE_ASSERT(probability >= 0.0f && probability <= 1.0f,
                "Random::Bool: probability must be in [0, 1]");
  return Float(0.0f, 1.0f) < probability;
}

} // namespace Engine::Core