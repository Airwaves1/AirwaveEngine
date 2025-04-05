#pragma once
#include <string>
#include <vector>

namespace Airwave
{
class FileUtils
{
  public:
    static bool exists(const std::string &path);
    static size_t fileSize(const std::string &path);
    static std::string readFile(const std::string &path);
    static std::vector<std::string> listDirectory(const std::string &path);
    static bool createFile(const std::string &path);
    static bool deleteFile(const std::string &path);
    static bool createDirectory(const std::string &path);
    static bool deleteDirectory(const std::string &path);
    static std::string getFileExtension(const std::string &path);
    static std::string getFileName(const std::string &path);
    static std::string getParentPath(const std::string &path);
};
} // namespace Airwave
