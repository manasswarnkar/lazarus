#include "FileSystem.h"
#include "Logger.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace Engine::Core {

std::optional<std::string>
FileSystem::ReadFileToString(const std::string &path) {
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.is_open()) {
    Logger::Warning(("Failed to open file: " + path).c_str());
    return std::nullopt;
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::optional<std::vector<uint8_t>>
FileSystem::ReadFileToBytes(const std::string &path) {
  std::ifstream file(path, std::ios::in | std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    Logger::Warning(("Failed to open file: " + path).c_str());
    return std::nullopt;
  }

  std::streamsize size = file.tellg();
  if (size < 0) {
    Logger::Warning(("Failed to determine file size: " + path).c_str());
    return std::nullopt;
  }
  file.seekg(0, std::ios::beg);

  std::vector<uint8_t> buffer(static_cast<size_t>(size));
  if (!file.read(reinterpret_cast<char *>(buffer.data()), size)) {
    Logger::Warning(("Failed to read file: " + path).c_str());
    return std::nullopt;
  }

  return buffer;
}

bool FileSystem::WriteStringToFile(const std::string &path,
                                   const std::string &contents) {
  std::ofstream file(path, std::ios::out | std::ios::binary);
  if (!file.is_open()) {
    Logger::Warning(("Failed to open file for writing: " + path).c_str());
    return false;
  }

  file << contents;
  return file.good();
}

bool FileSystem::Exists(const std::string &path) {
  std::error_code ec;
  return std::filesystem::exists(path, ec);
}

bool FileSystem::IsDirectory(const std::string &path) {
  std::error_code ec;
  return std::filesystem::is_directory(path, ec);
}

std::string FileSystem::GetExtension(const std::string &path) {
  return std::filesystem::path(path).extension().string();
}

std::string FileSystem::GetFileName(const std::string &path) {
  return std::filesystem::path(path).filename().string();
}

std::string FileSystem::GetFileNameWithoutExtension(const std::string &path) {
  return std::filesystem::path(path).stem().string();
}

bool FileSystem::CreateDirectories(const std::string &path) {
  std::error_code ec;
  std::filesystem::create_directories(path, ec);
  if (ec) {
    Logger::Warning(
        ("Failed to create directories: " + path + " (" + ec.message() + ")")
            .c_str());
    return false;
  }
  return true;
}

} // namespace Engine::Core