#pragma once

#include <cstdint>
#include <functional>

namespace Engine::Core {

// A 64-bit randomly-generated identifier. Not a full RFC 4122 UUID —
// just a random 64-bit integer, which is what most engines actually use
// under the "UUID" name for entity IDs, asset handles, etc. Collision
// probability is negligible for any realistic number of IDs a single
// engine session will generate.
class UUID {
public:
  UUID();
  explicit UUID(uint64_t uuid);

  operator uint64_t() const { return m_UUID; }

  bool operator==(const UUID &other) const { return m_UUID == other.m_UUID; }
  bool operator!=(const UUID &other) const { return !(*this == other); }

private:
  uint64_t m_UUID;
};

} // namespace Engine::Core

// Allows UUID to be used as a key in std::unordered_map/unordered_set.
namespace std {
template <> struct hash<Engine::Core::UUID> {
  size_t operator()(const Engine::Core::UUID &uuid) const noexcept {
    return hash<uint64_t>()(static_cast<uint64_t>(uuid));
  }
};
} // namespace std