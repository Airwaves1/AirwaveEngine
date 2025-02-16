#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <regex>
#include <set>

#include "resource/resource_manager.hpp"
#include "rendering/shader.hpp"
#include "utils/file_utils.hpp"

namespace Airwave
{
enum class ShaderKeyWord
{
    NONE                       = 0,
    USE_PBR                    = 1 << 0,
    USE_ALBEDO_MAP             = 1 << 1,
    USE_NORMAL_MAP             = 1 << 2,
    USE_METALLIC_ROUGHNESS_MAP = 1 << 3,
    USE_AO_MAP          = 1 << 4,
    USE_EMISSIVE_MAP           = 1 << 5,
    USE_IBL                    = 1 << 6,
    USE_SHADOW                 = 1 << 7,
    USE_TANGENT                = 1 << 8,
};

inline ShaderKeyWord operator|(ShaderKeyWord a, ShaderKeyWord b) { return static_cast<ShaderKeyWord>(static_cast<int>(a) | static_cast<int>(b)); }
inline ShaderKeyWord operator&(ShaderKeyWord a, ShaderKeyWord b) { return static_cast<ShaderKeyWord>(static_cast<int>(a) & static_cast<int>(b)); }
inline bool operator==(ShaderKeyWord a, ShaderKeyWord b) { return static_cast<int>(a) == static_cast<int>(b); }
inline bool operator!=(ShaderKeyWord a, ShaderKeyWord b) { return static_cast<int>(a) != static_cast<int>(b); }
inline bool HasKeyWord(ShaderKeyWord key, ShaderKeyWord word) { return (key & word) == word; }

class ShaderUtils
{
  public:
    static ShaderUtils &GetInstance()
    {
        static ShaderUtils instance;
        return instance;
    }

    const std::pair<std::string, std::string> separateShader(const std::string &shaderCode)
    {
        std::string vertexShader;
        std::string fragmentShader;
        std::string currentShader;

        std::istringstream sourceStream(shaderCode);
        std::string line;

        while (std::getline(sourceStream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                // 修改匹配关键字
                if (line.find("vert") != std::string::npos)
                {
                    currentShader = "vertex";
                }
                else if (line.find("frag") != std::string::npos)
                {
                    currentShader = "fragment";
                }
            }
            else
            {
                if (currentShader == "vertex")
                {
                    vertexShader += line + "\n";
                }
                else if (currentShader == "fragment")
                {
                    fragmentShader += line + "\n";
                }
            }
        }

        return {vertexShader, fragmentShader};
    }

    std::string process(const std::string &filePath, const std::unordered_map<std::string, std::string> &defines = {})
    {
        std::set<std::string> processedFiles;
        return processInternal(filePath, defines, true, processedFiles);
    }

    std::string process_shader_key_word(const std::string &shaderCode, ShaderKeyWord keyWord)
    {
        std::string result = shaderCode;
    
        // 1. 先删除已有的宏定义
        for (const auto &[key, value] : m_shader_key_word_defines)
        {
            size_t pos = result.find(value);
            if (pos != std::string::npos)
            {
                result.erase(pos, value.length());
            }
        }
    
        // 2. 生成当前需要的宏定义
        std::string defines;
        for (const auto &[key, value] : m_shader_key_word_defines)
        {
            if (HasKeyWord(keyWord, key))
            {
                defines += value;
            }
        }
    
        // 3. 确保 `#define` 在 `#version` 之后
        size_t versionPos = result.find("#version");
        if (versionPos != std::string::npos)
        {
            size_t lineEnd = result.find("\n", versionPos); // 找到 `#version` 这一行的结束位置
            if (lineEnd != std::string::npos)
            {
                result.insert(lineEnd + 1, defines); // 在 `#version` 后插入
            }
        }
        else
        {
            result = defines + result; // 如果没有 `#version`，直接插入
        }
    
        return result;
    }
    

    std::shared_ptr<Shader> get_new_variant_shader(const std::shared_ptr<Shader> &shader, ShaderKeyWord key)
    {
        if (m_shader_cache.find(key) != m_shader_cache.end())
        {
            return m_shader_cache[key];
        }

        std::string vertexShader;
        std::string fragmentShader;

        vertexShader = shader->getVertexSrc();
        fragmentShader = shader->getFragmentSrc();

        // 处理关键字
        vertexShader = process_shader_key_word(vertexShader, key);
        fragmentShader = process_shader_key_word(fragmentShader, key);

        auto newShader = std::make_shared<Shader>(vertexShader, fragmentShader);
        m_shader_cache[key] = newShader;

        return newShader;
    }

  private:
    ShaderUtils()
    {
        m_includePath = std::string(PROJECT_ROOT_DIR) + "/assets/shaders/";
        // 预置常用shader chunk路径
        m_shaderFileCache["common"] = std::string(PROJECT_ROOT_DIR) + "/assets/shaders/shader_chunk/common.glsl";
        m_shader_key_word_defines   = {
            {ShaderKeyWord::USE_PBR, "#define USE_PBR\n"},
            {ShaderKeyWord::USE_ALBEDO_MAP, "#define USE_ALBEDO_MAP\n"},
            {ShaderKeyWord::USE_NORMAL_MAP, "#define USE_NORMAL_MAP\n"},
            {ShaderKeyWord::USE_METALLIC_ROUGHNESS_MAP, "#define USE_METALLIC_ROUGHNESS_MAP\n"},
            {ShaderKeyWord::USE_AO_MAP, "#define USE_AO_MAP\n"},
            {ShaderKeyWord::USE_EMISSIVE_MAP, "#define USE_EMISSIVE_MAP\n"},
            {ShaderKeyWord::USE_IBL, "#define USE_IBL\n"},
            {ShaderKeyWord::USE_SHADOW, "#define USE_SHADOW\n"},
            {ShaderKeyWord::USE_TANGENT, "#define USE_TANGENT\n"},
        };
    }

    std::string processInternal(const std::string &filePath, const std::unordered_map<std::string, std::string> &defines, bool isTopLevel,
                                std::set<std::string> &processedFiles)
    {
        // 防止循环包含和重复处理
        if (processedFiles.find(filePath) != processedFiles.end())
        {
            return "";
        }
        processedFiles.insert(filePath);

        // 读取shader文件内容
        std::string sourceCode;
        if (!readFile(filePath, sourceCode))
        {
            // std::cerr << "Error: Failed to read shader file: " << filePath << std::endl;
            LOG_ERROR("Failed to read shader file: {0}", filePath);
            return "";
        }

        std::stringstream output;
        std::istringstream sourceStream(sourceCode);
        std::string line;

        // 添加宏定义到顶层文件
        if (isTopLevel)
        {
            for (const auto &[name, value] : defines)
            {
                output << "#define " << name << " " << value << "\n";
            }
        }

        // 正则表达式匹配#include指令
        std::regex includeRegex(R"(\s*#include\s+[<"](.+?)[">]\s*)");

        while (std::getline(sourceStream, line))
        {
            std::smatch match;

            // 处理include指令
            if (std::regex_match(line, match, includeRegex))
            {
                std::string includeName = match[1].str();

                // 从m_includePath查找路径
                std::string includePath = m_includePath + includeName;

                // 递归处理包含文件
                if (readFile(includePath, sourceCode))
                {
                    output << processInternal(includePath, defines, false, processedFiles) << "\n";
                }
                else
                {
                    std::cerr << "Warning: Missing include file - " << includeName << " (searched in: " << includePath << ")" << std::endl;
                }
            }
            // 保留原始代码
            else
            {
                output << line << "\n";
            }
        }

        return output.str();
    }

    bool readFile(const std::string &path, std::string &content)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
        return true;
    }

  private:
    std::string m_includePath;                                                // Shader资源路径
    std::unordered_map<std::string, std::string> m_shaderFileCache;           // Shader块路径缓存
    std::unordered_map<ShaderKeyWord, std::string> m_shader_key_word_defines; // Shader关键字宏定义

    std::unordered_map<ShaderKeyWord, std::shared_ptr<Shader>> m_shader_cache;
};
} // namespace Airwave