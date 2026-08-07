#include "Core/UUID.h"
#include <gtest/gtest.h>
#include <unordered_map>
#include <unordered_set>

using namespace Engine::Core;

TEST(UUIDTest, DefaultConstructedUUIDsAreDifferent) {
  UUID a;
  UUID b;
  EXPECT_NE(a, b);
}

TEST(UUIDTest, ExplicitValueIsPreserved) {
  UUID id(123456789);
  EXPECT_EQ(static_cast<uint64_t>(id), 123456789u);
}

TEST(UUIDTest, EqualityComparesUnderlyingValue) {
  UUID a(42);
  UUID b(42);
  UUID c(43);

  EXPECT_EQ(a, b);
  EXPECT_NE(a, c);
}

TEST(UUIDTest, ImplicitConversionToUint64Works) {
  UUID id(999);
  uint64_t raw = id; // implicit conversion via operator uint64_t
  EXPECT_EQ(raw, 999u);
}

TEST(UUIDTest, NoCollisionsAcrossManyGeneratedIDs) {
  constexpr int kCount = 100000;
  std::unordered_set<uint64_t> seen;
  seen.reserve(kCount);

  for (int i = 0; i < kCount; ++i) {
    UUID id;
    auto [it, inserted] = seen.insert(static_cast<uint64_t>(id));
    ASSERT_TRUE(inserted) << "Collision detected after " << i << " IDs";
  }
}

TEST(UUIDTest, UsableAsUnorderedMapKey) {
  UUID id;
  std::unordered_map<UUID, std::string> map;
  map[id] = "Player";

  EXPECT_EQ(map[id], "Player");
  EXPECT_EQ(map.count(id), 1u);
}

TEST(UUIDTest, DifferentUUIDsHaveDifferentHashesUsually) {
  // Not a strict guarantee (hash collisions are theoretically possible
  // even for different values), but for a healthy 64-bit hash, two
  // random UUIDs should hash differently in the overwhelming majority
  // of cases — useful as a smoke test that std::hash<UUID> is wired up
  // correctly and isn't, e.g., always returning 0.
  UUID a, b;
  std::hash<UUID> hasher;

  EXPECT_NE(hasher(a), hasher(b));
}