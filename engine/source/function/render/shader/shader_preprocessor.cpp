#include "shader_preprocessor.hpp"
#include "core/log/log.hpp"

namespace Airwave
{

std::string ShaderPreprocessor::preProcess(const std::string &source_code, bool isTopLevel,
                                           const std::vector<std::pair<std::string, std::string>> &macros,
                                           std::unordered_set<std::string> &include_stack)
{
    std::istringstream stream(source_code);
    std::ostringstream processed_code;
    std::string line;

    if (isTopLevel)
    {
        // 处理宏定义
        for (const auto &macro : macros)
        {
            processed_code << "#define " << macro.first << " " << macro.second << "\n";
        }
    }

    while (std::getline(stream, line))
    {
        if (line.find("#include") == 0)
        {
            // 提取文件名   
            size_t start = line.find('"');
            size_t end = line.find('"', start + 1);

            if (start != std::string::npos && end != std::string::npos && start < end)
            {
                std::string filename = line.substr(start + 1, end - start - 1);

                // 检查是否已经包含，防止循环依赖
                if (include_stack.find(filename) != include_stack.end())
                {
                    LOG_ERROR("Circular dependency detected in shader includes: {}", filename);
                    continue;
                }

                // 标记正在解析的 include 文件
                include_stack.insert(filename);

                std::string included_code;
                if (m_includes.find(filename) != m_includes.end())
                {
                    included_code = m_includes[filename];
                }
                else
                {
                    std::ifstream file;
                    std::string full_path;

                    // 遍历 m_include_paths 查找文件
                    for (const auto &path : m_include_paths)
                    {
                        std::string test_path = path + "/" + filename;
                        file.open(test_path);
                        if (file.is_open())
                        {
                            full_path = test_path;
                            break;
                        }
                    }

                    if (!file.is_open())
                    {
                        file.open(filename); // 直接尝试加载（绝对路径或当前目录）
                        full_path = filename;
                    }

                    if (file.is_open())
                    {
                        std::ostringstream file_content;
                        file_content << file.rdbuf();
                        included_code = file_content.str();

                        // 递归处理包含的文件
                        included_code = preProcess(included_code, false, macros, include_stack);

                        // 缓存文件内容
                        m_includes[filename] = included_code;
                    }
                    else
                    {
                        LOG_ERROR("Failed to open include file: {}", filename);
                        processed_code << line << "\n"; // 保留原始行
                        continue;
                    }
                }

                // 解析完成后移除
                include_stack.erase(filename);
                processed_code << included_code << "\n";
            }
        }
        else
        {
            processed_code << line << "\n";
        }
    }

    return processed_code.str();
}

// 入口函数（重载）
std::string ShaderPreprocessor::preProcess(const std::string &source_code, bool isTopLevel,
                                           const std::vector<std::pair<std::string, std::string>> &macros)
{
    std::unordered_set<std::string> include_stack; // 追踪当前包含的文件
    return preProcess(source_code, isTopLevel, macros, include_stack);
}

void ShaderPreprocessor::loadIncludeFile()
{
    for (const auto &include : m_include_paths)
    {
        std::string file = FileUtils::readFile(AssetsPath + "shader/shader_chunk/" + include);
        if (!file.empty())
        {
            m_includes[include] = file; // 缓存文件内容
        }
        else
        {
            LOG_ERROR("Failed to load include file: {}", AssetsPath + "shader/shader_chunk/" + include);
        }
    }
}

std::vector<std::pair<std::string, std::string>> ShaderPreprocessor::splitShader(const std::string &source_code)
{
    std::vector<std::pair<std::string, std::string>> shaders;
    std::istringstream stream(source_code);
    std::string line, current_type, current_code;

    while (std::getline(stream, line))
    {
        if (line.rfind("#shader", 0) == 0)
        { // 以 #shader 开头
            if (!current_type.empty())
            {
                // 保存当前类型和代码
                shaders.emplace_back(current_type, current_code);
            }

            // 清空当前代码
            current_code.clear();

            // 检查并确定着色器类型
            if (line.find("vertex") != std::string::npos)
            {
                current_type = "vertex";
            }
            else if (line.find("fragment") != std::string::npos)
            {
                current_type = "fragment";
            }
            else if (line.find("geometry") != std::string::npos)
            {
                current_type = "geometry";
            }
            else if (line.find("compute") != std::string::npos)
            {
                current_type = "compute";
            }
            else
            {
                LOG_ERROR("Unknown shader type found in line: {}", line);
                current_type.clear();
            }
        }
        else
        {
            // 将代码行添加到当前代码块
            current_code += line + "\n";
        }
    }

    // 确保最后一个着色器也被添加
    if (!current_type.empty())
    {
        shaders.emplace_back(current_type, current_code);
    }

    return shaders;
}


} // namespace Airwave
