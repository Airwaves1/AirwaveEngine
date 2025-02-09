#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <regex>
#include <set>

#include "resource/resource_manager.hpp"
#include "rendering/opengl/gl_shader.hpp"
#include "utils/file_utils.hpp"

namespace Airwave
{
class ShaderUtils
{
public:
    static ShaderUtils& GetInstance()
    {
        static ShaderUtils instance;
        return instance;
    }

    std::string process(const std::string& filePath,
                        const std::unordered_map<std::string, std::string>& defines = {})
    {
        std::set<std::string> processedFiles;
        return processInternal(filePath, defines, true, processedFiles);
    }

private:
    ShaderUtils()
    {
        m_includePath = std::string(PROJECT_ROOT_DIR) + "/assets/shaders/";
        // 预置常用shader chunk路径
        m_shaderFileCache["common"] = std::string(PROJECT_ROOT_DIR) + "/assets/shaders/shader_chunk/common.glsl";
    }

    std::string processInternal(const std::string& filePath,
                                const std::unordered_map<std::string, std::string>& defines,
                                bool isTopLevel,
                                std::set<std::string>& processedFiles)
    {
        // 防止循环包含和重复处理
        if (processedFiles.find(filePath) != processedFiles.end()) {
            return "";
        }
        processedFiles.insert(filePath);

        // 读取shader文件内容
        std::string sourceCode;
        if (!readFile(filePath, sourceCode)) {
            // std::cerr << "Error: Failed to read shader file: " << filePath << std::endl;
            LOG_ERROR("Failed to read shader file: {0}", filePath);
            return "";
        }

        std::stringstream output;
        std::istringstream sourceStream(sourceCode);
        std::string line;

        // 添加宏定义到顶层文件
        if (isTopLevel) {
            for (const auto& [name, value] : defines) {
                output << "#define " << name << " " << value << "\n";
            }
        }

        // 正则表达式匹配#include指令
        std::regex includeRegex(R"(\s*#include\s+[<"](.+?)[">]\s*)");

        while (std::getline(sourceStream, line)) {
            std::smatch match;
            
            // 处理include指令
            if (std::regex_match(line, match, includeRegex)) {
                std::string includeName = match[1].str();

                // 从m_includePath查找路径
                std::string includePath = m_includePath + includeName;

                // 递归处理包含文件
                if (readFile(includePath, sourceCode)) {
                    output << processInternal(includePath, defines, false, processedFiles) << "\n";
                } else {
                    std::cerr << "Warning: Missing include file - " << includeName 
                              << " (searched in: " << includePath << ")" << std::endl;
                }
            } 
            // 保留原始代码
            else {
                output << line << "\n";
            }
        }

        return output.str();
    }

    bool readFile(const std::string& path, std::string& content)
    {
        std::ifstream file(path);
        if (!file.is_open()) {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
        return true;
    }

private:
    std::string m_includePath; // Shader资源路径
    std::unordered_map<std::string, std::string> m_shaderFileCache; // Shader块路径缓存
};
} // namespace Airwave