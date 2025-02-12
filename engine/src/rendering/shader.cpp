#include "rendering/shader.hpp"

#include "core/log.hpp"
#include "utils/shader_utils.hpp"

namespace Airwave
{

std::shared_ptr<Shader> Shader::create_default_shader()
{
    static std::shared_ptr<Shader> defaultShader = std::make_shared<Shader>(
        R"(
    #version 460 core
    layout(location = 0) in vec3 a_position;
    void main()
    {
        gl_Position = vec4(a_position, 1.0);
    }
    )",
        R"(
    #version 460 core
    out vec4 fragColor;
    void main()
    {
        fragColor = vec4(0.5, 0.45, 0.3, 1.0);
    }
    )");
    return defaultShader;
}

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc)
{
    m_vertexSrc   = vertexSrc;
    m_fragmentSrc = fragmentSrc;

    uint32_t vertexShader   = compile(m_vertexSrc, GL_VERTEX_SHADER);
    uint32_t fragmentShader = compile(m_fragmentSrc, GL_FRAGMENT_SHADER);

    m_handle = link(vertexShader, fragmentShader);

    if (m_handle == 0)
    {
        LOG_ERROR("Shader {0} failed to compile", vertexSrc + "\n" + fragmentSrc);
    }
}

Shader::~Shader()
{
    if (m_handle != 0)
    {
        glDeleteProgram(m_handle);
    }
}

uint32_t Shader::compile(const std::string &source, GLenum shaderType)
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
        LOG_ERROR("Shader compilation failed: {0}, \n source: \n{1}\n", infoLog, source);
    }

    return shader;
}

uint32_t Shader::link(const uint32_t &vertexShader, const uint32_t &fragmentShader)
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
        LOG_ERROR("Shader linking failed: {0},\n source: \n{1}\n", infoLog, m_vertexSrc + "\n" + m_fragmentSrc);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void Airwave::Shader::add_defines(std::vector<std::pair<std::string, std::string>> defines) {}

} // namespace Airwave
