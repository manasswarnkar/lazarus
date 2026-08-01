#include "Core/LinearAllocator.h"
#include <cstdint>
#include <gtest/gtest.h>

using namespace Engine::Core;

TEST(LinearAllocatorTest, AllocatesWithinCapacity) {
  LinearAllocator allocator(1024);

  void *ptr = allocator.Allocate(64);
  EXPECT_NE(ptr, nullptr);
  EXPECT_EQ(allocator.UsedBytes(), 64u);
}

TEST(LinearAllocatorTest, ReturnsNullptrWhenOutOfSpace) {
  LinearAllocator allocator(64);

  void *first = allocator.Allocate(64);
  EXPECT_NE(first, nullptr);

  void *second = allocator.Allocate(1); // no room left
  EXPECT_EQ(second, nullptr);
}

TEST(LinearAllocatorTest, SequentialAllocationsDoNotOverlap) {
  LinearAllocator allocator(256);

  auto *a = static_cast<uint8_t *>(allocator.Allocate(16));
  auto *b = static_cast<uint8_t *>(allocator.Allocate(16));

  ASSERT_NE(a, nullptr);
  ASSERT_NE(b, nullptr);

  // b must start at or after where a's block ends
  EXPECT_GE(b, a + 16);
}

TEST(LinearAllocatorTest, ResetReclaimsAllSpace) {
  LinearAllocator allocator(128);

  allocator.Allocate(128); // fill it completely
  EXPECT_EQ(allocator.UsedBytes(), 128u);

  allocator.Reset();
  EXPECT_EQ(allocator.UsedBytes(), 0u);

  void *ptr = allocator.Allocate(128); // should succeed again after reset
  EXPECT_NE(ptr, nullptr);
}

TEST(LinearAllocatorTest, AllocationsAreCorrectlyAligned) {
  LinearAllocator allocator(1024);

  // Force an odd initial offset...
  allocator.Allocate(1);

  // ...then request a 16-byte-aligned block
  void *ptr = allocator.Allocate(16, 16);
  ASSERT_NE(ptr, nullptr);

  auto address = reinterpret_cast<uintptr_t>(ptr);
  EXPECT_EQ(address % 16, 0u);
}

TEST(LinearAllocatorTest, WrittenDataIsPreservedUntilReset) {
  LinearAllocator allocator(256);

  auto *data = static_cast<int *>(allocator.Allocate(sizeof(int)));
  ASSERT_NE(data, nullptr);
  *data = 42;

  EXPECT_EQ(*data, 42);
}

TEST(LinearAllocatorTest, MoveConstructionTransfersOwnership) {
  LinearAllocator original(128);
  void *ptr = original.Allocate(64);
  EXPECT_NE(ptr, nullptr);

  LinearAllocator moved(std::move(original));
  EXPECT_EQ(moved.UsedBytes(), 64u);
  EXPECT_EQ(moved.CapacityBytes(), 128u);
}