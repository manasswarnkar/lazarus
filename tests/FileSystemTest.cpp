#include "Core/FileSystem.h"
#include <filesystem>
#include <gtest/gtest.h>

using namespace Engine::Core;

namespace {
std::string TempPath(const std::string &name) {
  return (std::filesystem::temp_directory_path() / name).string();
}
} // namespace

TEST(FileSystemTest, ExistsReturnsFalseForMissingFile) {
  EXPECT_FALSE(FileSystem::Exists(TempPath("definitely_not_here_12345.txt")));
}

TEST(FileSystemTest, WriteThenExistsReturnsTrue) {
  std::string path = TempPath("fs_test_exists.txt");
  ASSERT_TRUE(FileSystem::WriteStringToFile(path, "hello"));

  EXPECT_TRUE(FileSystem::Exists(path));

  std::filesystem::remove(path);
}

TEST(FileSystemTest, WriteThenReadRoundTrips) {
  std::string path = TempPath("fs_test_roundtrip.txt");
  std::string content = "Hello, Lazarus Engine!\nSecond line.";

  ASSERT_TRUE(FileSystem::WriteStringToFile(path, content));

  auto result = FileSystem::ReadFileToString(path);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(*result, content);

  std::filesystem::remove(path);
}

TEST(FileSystemTest, ReadFileToStringReturnsNulloptForMissingFile) {
  auto result = FileSystem::ReadFileToString(TempPath("missing_file_xyz.txt"));
  EXPECT_FALSE(result.has_value());
}

TEST(FileSystemTest, ReadFileToBytesRoundTrips) {
  std::string path = TempPath("fs_test_bytes.bin");
  std::string content = "binary-ish content 123";
  ASSERT_TRUE(FileSystem::WriteStringToFile(path, content));

  auto result = FileSystem::ReadFileToBytes(path);
  ASSERT_TRUE(result.has_value());
  EXPECT_EQ(result->size(), content.size());

  std::string reconstructed(result->begin(), result->end());
  EXPECT_EQ(reconstructed, content);

  std::filesystem::remove(path);
}

TEST(FileSystemTest, ReadFileToBytesReturnsNulloptForMissingFile) {
  auto result = FileSystem::ReadFileToBytes(TempPath("missing_bytes_xyz.bin"));
  EXPECT_FALSE(result.has_value());
}

TEST(FileSystemTest, GetExtensionReturnsDotPrefixedExtension) {
  EXPECT_EQ(FileSystem::GetExtension("assets/textures/foo.png"), ".png");
  EXPECT_EQ(FileSystem::GetExtension("shader.vert"), ".vert");
}

TEST(FileSystemTest, GetExtensionReturnsEmptyForNoExtension) {
  EXPECT_EQ(FileSystem::GetExtension("assets/README"), "");
}

TEST(FileSystemTest, GetFileNameReturnsNameWithExtension) {
  EXPECT_EQ(FileSystem::GetFileName("assets/textures/foo.png"), "foo.png");
}

TEST(FileSystemTest, GetFileNameWithoutExtensionStripsExtension) {
  EXPECT_EQ(FileSystem::GetFileNameWithoutExtension("assets/textures/foo.png"),
            "foo");
}

TEST(FileSystemTest, IsDirectoryDistinguishesFilesFromDirectories) {
  std::string dirPath = TempPath("fs_test_dir");
  std::string filePath = TempPath("fs_test_dir_marker.txt");

  FileSystem::CreateDirectories(dirPath);
  FileSystem::WriteStringToFile(filePath, "x");

  EXPECT_TRUE(FileSystem::IsDirectory(dirPath));
  EXPECT_FALSE(FileSystem::IsDirectory(filePath));

  std::filesystem::remove(filePath);
  std::filesystem::remove(dirPath);
}

TEST(FileSystemTest, CreateDirectoriesCreatesNestedPath) {
  std::string nestedPath = TempPath("fs_test_nested/a/b/c");

  EXPECT_TRUE(FileSystem::CreateDirectories(nestedPath));
  EXPECT_TRUE(FileSystem::Exists(nestedPath));
  EXPECT_TRUE(FileSystem::IsDirectory(nestedPath));

  std::filesystem::remove_all(TempPath("fs_test_nested"));
}