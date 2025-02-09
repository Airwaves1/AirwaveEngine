#include "resource/shader_resource.hpp"

#include <glad/glad.h>

#include "utils/shader_utils.hpp"
#include "core/log.hpp"

namespace Airwave
{
ShaderResource::~ShaderResource()
{
    if (m_handle != 0)
    {
        glDeleteProgram(m_handle);
    }

    ShaderResourceEvent event{m_handle, m_uuid};

    m_eventDispatcher->dispatch(event);
}

bool ShaderResource::load(const std::string &vertexPath, const std::string &fragmentPath)
{
    m_vertexPath   = vertexPath;
    m_fragmentPath = fragmentPath;

    m_vertexShaderSource   = ShaderUtils::GetInstance().process(vertexPath);
    m_fragmentShaderSource = ShaderUtils::GetInstance().process(fragmentPath);

    uint32_t vertexShader   = compile(m_vertexShaderSource, GL_VERTEX_SHADER);
    uint32_t fragmentShader = compile(m_fragmentShaderSource, GL_FRAGMENT_SHADER);

    m_handle = link(vertexShader, fragmentShader);

    return m_handle != 0;
}

uint32_t ShaderResource::compile(const std::string &source, GLenum shaderType)
{
    uint32_t shader = glCreateShader(shaderType); // 创建 Shader 对象
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string path = shaderType == GL_VERTEX_SHADER ? m_vertexPath : m_fragmentPath;
        LOG_ERROR("Shader {0} compilation failed: {1}", path, infoLog);
    }

    return shader;
}

uint32_t ShaderResource::link(const uint32_t &vertexShader, const uint32_t &fragmentShader)
{
    uint32_t program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        LOG_ERROR("Shader linking failed: {0}", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

bool ShaderResource::reload(const std::string &vertexPath, const std::string &fragmentPath)
{
    return false;
}

} // namespace Airwave
