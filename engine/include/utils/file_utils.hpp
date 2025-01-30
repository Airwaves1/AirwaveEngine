#pragma once

#include <string>

namespace Airwave
{
class FileUtils
{
  public:
    // 读取文件的内容
    static std::string ReadFile(const std::string &filepath);

    // 获取文件大小
    static size_t GetFileSize(const std::string &filepath);

  private:
    FileUtils() = delete;
};

} // namespace Airwave