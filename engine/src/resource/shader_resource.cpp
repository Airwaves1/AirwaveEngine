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

bool ShaderResource::onLoad(const std::string &path, const std::any &params)
{
    m_path = path;
    std::string shaderCode                      = ShaderUtils::GetInstance().process(path);
    auto [vertexShaderCode, fragmentShaderCode] = ShaderUtils::GetInstance().separateShader(shaderCode);

    m_vertexShaderSource   = vertexShaderCode;
    m_fragmentShaderSource = fragmentShaderCode;


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
        LOG_ERROR("Shader {0} compilation failed: {1}", m_path, infoLog);
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
        LOG_ERROR("Shader {0} linking failed: {1}",m_path, infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

bool ShaderResource::reload(const std::string &vertexPath, const std::string &fragmentPath) { return false; }

} // namespace Airwave
