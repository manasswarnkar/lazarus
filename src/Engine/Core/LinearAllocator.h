#pragma once

#include <cstddef>
#include <cstdint>

namespace Engine::Core {

// A simple bump-pointer allocator. Allocates from a fixed-size block of
// memory by advancing an offset; individual allocations cannot be freed —
// call Reset() to reclaim the entire block at once.
//
// Typical use: per-frame scratch memory, reset at the start/end of each frame.

class LinearAllocator {
public:
  explicit LinearAllocator(std::size_t capacityBytes);
  ~LinearAllocator();

  // Non-copyable: owns a raw memory block.
  LinearAllocator(const LinearAllocator &) = delete;
  LinearAllocator &operator=(const LinearAllocator &) = delete;

  // Movable.
  LinearAllocator(LinearAllocator &&other) noexcept;
  LinearAllocator &operator=(LinearAllocator &&other) noexcept;

  // Allocates `size` bytes, aligned to `alignment` (must be a power of 2).
  // Returns nullptr if there isn't enough space left.
  void *Allocate(size_t size, size_t alignment = alignof(std::max_align_t));

  // Reclaims all allocations at once. Does NOT call destructors — this
  // allocator is for POD/trivially-destructible data unless the caller
  // manages destruction manually.
  void Reset();

  size_t CapacityBytes() const { return m_CapacityBytes; }
  size_t UsedBytes() const { return m_Offset; }

private:
  uint8_t *m_Base = nullptr;
  size_t m_CapacityBytes = 0;
  size_t m_Offset = 0;
};

} // namespace Engine::Core