#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

#include "function/global/global_context.hpp"
#include "platform/file/file_utils.hpp"

namespace Airwave
{
class ShaderPreprocessor
{
  public:
    static ShaderPreprocessor &getInstance()
    {
        static ShaderPreprocessor instance;
        return instance;
    }

    void loadIncludeFile();
    std::vector<std::pair<std::string, std::string>> splitShader(const std::string &source_code);
    std::string preProcess(const std::string &source_code, bool isTopLevel, const std::vector<std::pair<std::string, std::string>> &macros,
                           std::unordered_set<std::string> &include_stack);
    std::string preProcess(const std::string &source_code, bool isTopLevel = true, const std::vector<std::pair<std::string, std::string>> &macros = {});

  private:
    ShaderPreprocessor()
    {
        loadIncludeFile(); // 预加载一些常用的 include 文件
    }
    ~ShaderPreprocessor() = default;

    std::vector<std::string> m_include_paths = {"common.glsl"};
    std::unordered_map<std::string, std::string> m_includes; // include 文件缓存
};

} // namespace Airwave
