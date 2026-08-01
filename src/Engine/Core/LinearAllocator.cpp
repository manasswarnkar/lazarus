#include "LinearAllocator.h"
#include "Assert.h"
#include <cstdlib>

namespace Engine::Core {

namespace {
size_t AlignUp(size_t value, size_t alignment) {
  return (value + alignment - 1) & ~(alignment - 1);
}
} // namespace

LinearAllocator::LinearAllocator(size_t capacityBytes)
    : m_CapacityBytes(capacityBytes) {
  ENGINE_ASSERT(capacityBytes > 0, "LinearAllocator capacity must be > 0");
  m_Base = static_cast<uint8_t *>(std::malloc(capacityBytes));
  ENGINE_ASSERT(m_Base != nullptr,
                "LinearAllocator failed to allocate backing memory");
}

LinearAllocator::~LinearAllocator() { std::free(m_Base); }

LinearAllocator::LinearAllocator(LinearAllocator &&other) noexcept
    : m_Base(other.m_Base), m_CapacityBytes(other.m_CapacityBytes),
      m_Offset(other.m_Offset) {
  other.m_Base = nullptr;
  other.m_CapacityBytes = 0;
  other.m_Offset = 0;
}

LinearAllocator &LinearAllocator::operator=(LinearAllocator &&other) noexcept {
  if (this != &other) {
    std::free(m_Base);
    m_Base = other.m_Base;
    m_CapacityBytes = other.m_CapacityBytes;
    m_Offset = other.m_Offset;

    other.m_Base = nullptr;
    other.m_CapacityBytes = 0;
    other.m_Offset = 0;
  }
  return *this;
}

void *LinearAllocator::Allocate(size_t size, size_t alignment) {
  ENGINE_ASSERT((alignment & (alignment - 1)) == 0,
                "Alignment must be a power of 2");

  const size_t alignedOffset = AlignUp(m_Offset, alignment);
  const size_t newOffset = alignedOffset + size;

  if (newOffset > m_CapacityBytes) {
    return nullptr; // out of space — caller decides how to handle it
  }

  void *result = m_Base + alignedOffset;
  m_Offset = newOffset;
  return result;
}

void LinearAllocator::Reset() { m_Offset = 0; }

} // namespace Engine::Core