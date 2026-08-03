#include "Renderer/Buffer.h"
#include <gtest/gtest.h>

using namespace Engine::Renderer;

// ---------- ShaderDataTypeSize ----------

TEST(ShaderDataTypeSizeTest, ReturnsCorrectByteSizes) {
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Float), 4u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Float2), 8u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Float3), 12u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Float4), 16u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Int), 4u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Int2), 8u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Int3), 12u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Int4), 16u);
  EXPECT_EQ(ShaderDataTypeSize(ShaderDataType::Bool), 1u);
}

// ---------- BufferElement ----------

TEST(BufferElementTest, ComputesCorrectComponentCount) {
  EXPECT_EQ(BufferElement(ShaderDataType::Float, "x").GetComponentCount(), 1u);
  EXPECT_EQ(BufferElement(ShaderDataType::Float2, "x").GetComponentCount(), 2u);
  EXPECT_EQ(BufferElement(ShaderDataType::Float3, "x").GetComponentCount(), 3u);
  EXPECT_EQ(BufferElement(ShaderDataType::Float4, "x").GetComponentCount(), 4u);
  EXPECT_EQ(BufferElement(ShaderDataType::Int3, "x").GetComponentCount(), 3u);
  EXPECT_EQ(BufferElement(ShaderDataType::Bool, "x").GetComponentCount(), 1u);
}

TEST(BufferElementTest, StoresNameAndNormalizedFlag) {
  BufferElement element(ShaderDataType::Float3, "a_Position", true);

  EXPECT_EQ(element.Name, "a_Position");
  EXPECT_TRUE(element.Normalized);
  EXPECT_EQ(element.Size, 12u);
}

TEST(BufferElementTest, DefaultsToNotNormalized) {
  BufferElement element(ShaderDataType::Float3, "a_Position");
  EXPECT_FALSE(element.Normalized);
}

// ---------- BufferLayout ----------

TEST(BufferLayoutTest, SingleElementHasZeroOffsetAndMatchingStride) {
  BufferLayout layout = {{ShaderDataType::Float3, "a_Position"}};

  auto it = layout.begin();
  EXPECT_EQ(it->Offset, 0u);
  EXPECT_EQ(layout.GetStride(), 12u);
}

TEST(BufferLayoutTest, MultipleElementsHaveSequentialOffsets) {
  BufferLayout layout = {
      {ShaderDataType::Float3, "a_Position"}, // offset 0,  size 12
      {ShaderDataType::Float4, "a_Color"},    // offset 12, size 16
      {ShaderDataType::Float2, "a_TexCoord"}, // offset 28, size 8
  };

  auto it = layout.begin();
  EXPECT_EQ(it->Offset, 0u);
  ++it;
  EXPECT_EQ(it->Offset, 12u);
  ++it;
  EXPECT_EQ(it->Offset, 28u);
}

TEST(BufferLayoutTest, StrideEqualsSumOfElementSizes) {
  BufferLayout layout = {
      {ShaderDataType::Float3, "a_Position"}, // 12
      {ShaderDataType::Float4, "a_Color"},    // 16
      {ShaderDataType::Float2, "a_TexCoord"}, // 8
  };

  EXPECT_EQ(layout.GetStride(), 36u); // 12 + 16 + 8
}

TEST(BufferLayoutTest, EmptyLayoutHasZeroStride) {
  BufferLayout layout;
  EXPECT_EQ(layout.GetStride(), 0u);
  EXPECT_TRUE(layout.GetElements().empty());
}

TEST(BufferLayoutTest, PreservesElementOrder) {
  BufferLayout layout = {
      {ShaderDataType::Float3, "a_Position"},
      {ShaderDataType::Float4, "a_Color"},
  };

  auto it = layout.begin();
  EXPECT_EQ(it->Name, "a_Position");
  ++it;
  EXPECT_EQ(it->Name, "a_Color");
}

TEST(BufferLayoutTest, ElementCountMatchesInitializerList) {
  BufferLayout layout = {
      {ShaderDataType::Float3, "a_Position"},
      {ShaderDataType::Float4, "a_Color"},
      {ShaderDataType::Float2, "a_TexCoord"},
  };

  EXPECT_EQ(layout.GetElements().size(), 3u);
}