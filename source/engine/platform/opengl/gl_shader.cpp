#include "gl_shader.hpp"
#include "utils/file_utils.hpp"
#include <glm/gtc/type_ptr.hpp>
#define UNIFORM_DEBUG 1

namespace Airwave
{
Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc, bool fromFile,
               const std::vector<std::string> &defines)
    : m_handle(0)
{
    compile(vertexSrc, fragmentSrc, fromFile, defines);
}

Shader::~Shader()
{
    if (m_handle != 0)
    {
        glDeleteProgram(m_handle);
    }
}

void Shader::bind() const { glUseProgram(m_handle); }

void Shader::unbind() const { glUseProgram(0); }

void Shader::compile(const std::string &vertexSrc, const std::string &fragmentSrc, bool fromFile,
                     const std::vector<std::string> &defines)
{
    std::string vertexSource   = fromFile ? FileUtils::ReadFile(vertexSrc) : vertexSrc;
    std::string fragmentSource = fromFile ? FileUtils::ReadFile(fragmentSrc) : fragmentSrc;

    m_sources[ShaderType::VERTEX]   = vertexSource;
    m_sources[ShaderType::FRAGMENT] = fragmentSource;

    if (!defines.empty())
    {
        addDefine(defines);
    }

    m_handle = glCreateProgram();

    uint32_t vertexShader   = compileShader(vertexSource, ShaderType::VERTEX);
    uint32_t fragmentShader = compileShader(fragmentSource, ShaderType::FRAGMENT);

    glAttachShader(m_handle, vertexShader);
    glAttachShader(m_handle, fragmentShader);

    glLinkProgram(m_handle);

    int success;
    glGetProgramiv(m_handle, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(m_handle, 512, NULL, infoLog);
        LOG_ERROR("Shader program linking failed: {0}", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::recomplie() { compile(m_sources[ShaderType::VERTEX], m_sources[ShaderType::FRAGMENT], false, m_defines); }

void Shader::addDefine(const std::vector<std::string> &defines)
{
    for (const auto &define : defines)
    {
        if (std::find(m_defines.begin(), m_defines.end(), define) == m_defines.end())
        {
            m_defines.push_back(define);
            for (auto &source : m_sources)
            {
                // 找到 #version 的位置
                size_t versionPos = source.second.find("#version");
                if (versionPos != std::string::npos)
                {
                    // 在 #version 后插入定义
                    size_t insertPos = source.second.find('\n', versionPos) + 1;
                    source.second.insert(insertPos, "#define " + define + "\n");
                }
                else
                {
                    // 如果没有找到 #version，直接添加到源代码的开头
                    source.second = "#define " + define + "\n" + source.second;
                }
            }
        }
    }
}

void Shader::removeDefine(const std::string &define)
{
    auto it = std::remove(m_defines.begin(), m_defines.end(), define);
    m_defines.erase(it, m_defines.end());

    for (auto &source : m_sources)
    {
        std::string defineStr = "#define " + define;
        size_t pos;
        while ((pos = source.second.find(defineStr)) != std::string::npos)
        {
            size_t endPos = source.second.find('\n', pos) + 1; // 找到行末
            source.second.erase(pos, endPos - pos);            // 删除整行
        }
    }
}

uint32_t Shader::compileShader(const std::string &source, ShaderType type)
{

    const char *src = source.c_str();

    uint32_t shader = glCreateShader(static_cast<uint32_t>(type));

    glShaderSource(shader, 1, &src, NULL);

    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_ERROR("Shader compilation failed: {0}", infoLog);
    }

    return shader;
}

uint32_t Shader::getUniformLocation(const std::string &name)
{
    if (m_uniformLocationsCache.find(name) != m_uniformLocationsCache.end())
    {
        return m_uniformLocationsCache[name];
    }

    GLint location = glGetUniformLocation(m_handle, name.c_str());
    if (location == -1)
    {
        if (m_uniformErrordCache.find(name) == m_uniformErrordCache.end())
        {
#if UNIFORM_DEBUG
            LOG_ERROR("Uniform {0} not found!", name);
#endif
            m_uniformErrordCache[name] = true;
        }

        return -1;
    }

    m_uniformLocationsCache[name] = location;

    return location;
}

void Shader::uploadUniformBool(const std::string &name, bool value) {
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform1i(location, value);
    }
}

void Shader::uploadUniformInt(const std::string &name, int value) {
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform1i(location, value);
    }
}

void Shader::uploadUniformInt2(const std::string &name, const glm::ivec2 &value) {
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform2i(location, value.x, value.y);
    }
}

void Shader::uploadUniformFloat(const std::string &name, float value) {
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform1f(location, value);
    }
}

void Shader::uploadUniformFloat2(const std::string &name, const glm::vec2 &value) {
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform2f(location, value.x, value.y);
    }
}

void Shader::uploadUniformFloat3(const std::string &name, const glm::vec3 &value) {
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void Shader::uploadUniformFloat4(const std::string &name, const glm::vec4 &value) {
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
}

void Shader::uploadUniformMat3(const std::string &name, const glm::mat3 &matrix) {
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

void Shader::uploadUniformMat4(const std::string &name, const glm::mat4 &matrix) {
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
}

} // namespace Airwave
