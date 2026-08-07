#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace Engine::Core {

class FileSystem {
public:
  // Reads the entire file as text into a string. Returns std::nullopt if
  // the file doesn't exist or can't be opened — this is an expected,
  // recoverable failure (missing/bad path), not asserted.
  static std::optional<std::string> ReadFileToString(const std::string &path);

  // Reads the entire file as raw bytes. Returns std::nullopt on failure.
  static std::optional<std::vector<uint8_t>>
  ReadFileToBytes(const std::string &path);

  // Writes `contents` to `path`, overwriting any existing file. Returns
  // true on success.
  static bool WriteStringToFile(const std::string &path,
                                const std::string &contents);

  static bool Exists(const std::string &path);
  static bool IsDirectory(const std::string &path);

  // Returns the extension including the leading dot (e.g. ".png"), or
  // an empty string if the path has none.
  static std::string GetExtension(const std::string &path);

  // Returns the filename without its directory path (e.g. "foo.png"
  // from "assets/textures/foo.png").
  static std::string GetFileName(const std::string &path);

  // Returns the filename without its extension (e.g. "foo" from
  // "assets/textures/foo.png").
  static std::string GetFileNameWithoutExtension(const std::string &path);

  static bool CreateDirectories(const std::string &path);
};

} // namespace Engine::Core