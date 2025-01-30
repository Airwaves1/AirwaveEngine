#include "utils/file_utils.hpp"

#include <fstream>

#include "core/log.hpp"

namespace Airwave
{
std::string FileUtils::ReadFile(const std::string &filepath)
{
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in)
    {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    }
    else
    {
        LOG_ERROR("Could not open file: {0}", filepath);
    }

    return result;
}

size_t FileUtils::GetFileSize(const std::string &filepath)
{
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (!in)
    {
        LOG_ERROR("Could not open file: {0}", filepath);
        return 0;
    }

    in.seekg(0, std::ios::end);
    size_t size = in.tellg();
    in.close();

    return size;
}

} // namespace Airwave
