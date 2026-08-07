#include "UUID.h"
#include <random>

namespace Engine::Core {

namespace {
// A single, shared random engine + distribution for UUID generation.
// std::random_device seeds a Mersenne Twister once at startup; the
// generator itself keeps internal state between calls, so we don't
// reseed on every UUID() construction (which would be both slow and,
// counterintuitively, worse for randomness quality).
std::random_device s_RandomDevice;
std::mt19937_64 s_Engine(s_RandomDevice());
std::uniform_int_distribution<uint64_t> s_Distribution;
} // namespace

UUID::UUID() : m_UUID(s_Distribution(s_Engine)) {}

UUID::UUID(uint64_t uuid) : m_UUID(uuid) {}

} // namespace Engine::Core