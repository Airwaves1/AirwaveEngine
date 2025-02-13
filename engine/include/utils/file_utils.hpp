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

    // 判断文件是否存在
    static bool Exists(const std::string &filepath);

    // 获取文件所在目录
    static std::string GetDirectory(const std::string &filepath);

    // 截取/assets/后的路径
    static std::string GetAssetPath(const std::string &filepath);

  private:
    FileUtils() = delete;
};

} // namespace Airwave