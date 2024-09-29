#pragma once

#include <string>
#include <fstream>
#include "log.hpp"

#ifdef ASSETS_DIR
#define SHADER_DIR ASSETS_DIR "shaders/"
#define TEXTURE_DIR ASSETS_DIR "textures/"
#define MODEL_DIR ASSETS_DIR "models/"
#endif

namespace Airwave
{
class FileUtils
{
  public:
    static std::string ReadFile(const std::string &path)
    {
        std::string result;
        std::ifstream in(path, std::ios::in | std::ios::binary);
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
            LOG_ERROR("Could not open file '{0}'", path);
        }
        return result;
    }

  private:
    FileUtils() = delete;
};

} // namespace Airwave
