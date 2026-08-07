#pragma once

#include <cstdint>

namespace Engine::Core {

// Static, seedable random number generator for general engine/gameplay
// use. Auto-seeded from std::random_device at first use; call Seed()
// explicitly for reproducible sequences (testing, deterministic replay,
// procedural generation with a known seed).
class Random {
public:
  // Reseeds the generator. Two calls to Seed(sameValue) followed by
  // identical sequences of Random:: calls will produce identical results.
  static void Seed(uint32_t seed);

  // Returns a float in [min, max).
  static float Float(float min = 0.0f, float max = 1.0f);

  // Returns an int in [min, max] (inclusive on both ends).
  static int Int(int min, int max);

  // Returns true with the given probability (0.0 = never, 1.0 = always).
  static bool Bool(float probability = 0.5f);

private:
  static void EnsureInitialized();
};

} // namespace Engine::Core