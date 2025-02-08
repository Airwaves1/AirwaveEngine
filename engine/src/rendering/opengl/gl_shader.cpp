#include "rendering/opengl/gl_shader.hpp"
#include "utils/file_utils.hpp"
#include "core/log.hpp"
#include "utils/shader_utils.hpp"

namespace Airwave
{
Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc, bool fromFile)
{
    // if (fromFile)
    // {
    //     m_vertexShaderSource   = FileUtils::ReadFile(vertexSrc);
    //     m_fragmentShaderSource = FileUtils::ReadFile(fragmentSrc);
    // }
    // else
    // {
    //     m_vertexShaderSource   = vertexSrc;
    //     m_fragmentShaderSource = fragmentSrc;
    // }

    m_vertexShaderSource = ShaderUtils::GetInstance().process(vertexSrc);
    m_fragmentShaderSource = ShaderUtils::GetInstance().process(fragmentSrc);

    uint32_t vertexShader   = compile(m_vertexShaderSource, GL_VERTEX_SHADER);
    uint32_t fragmentShader = compile(m_fragmentShaderSource, GL_FRAGMENT_SHADER);

    m_handle = link(vertexShader, fragmentShader);
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
        LOG_ERROR("Shader compilation failed: {0}", infoLog);
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
        LOG_ERROR("Shader linking failed: {0}", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void Shader::addDefine(const std::string &name, const std::string &value) {

}
void Shader::setDefineValue(const std::string &name, const std::string &value) {}
void Shader::removeDefine(const std::string &name) {}

uint32_t Shader::getUniformLocation(const std::string &name)
{
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
    {
        return m_uniformLocationCache[name];
    }

    GLint location = glGetUniformLocation(m_handle, name.c_str());
    if (location == -1)
    {
        if (m_uniformErrordCache.find(name) == m_uniformErrordCache.end())
        {
            // LOG_WARN("Uniform {0} not found!", name);
            m_uniformErrordCache[name] = true;
        }

        return -1;
    }

    m_uniformLocationCache[name] = location;
    return location;
}

void Shader::setUniformBool(const std::string &name, bool value)
{
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform1i(location, (int)value);
    }
}

void Shader::setUniformInt(const std::string &name, int value)
{
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform1i(location, value);
    }
}

void Shader::setUniformFloat(const std::string &name, float value)
{
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform1f(location, value);
    }
}

void Shader::setUniformFloat2(const std::string &name, const glm::vec2 &value)
{
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform2f(location, value.x, value.y);
    }
}

void Shader::setUniformFloat3(const std::string &name, const glm::vec3 &value)
{
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform3f(location, value.x, value.y, value.z);
    }
}

void Shader::setUniformFloat4(const std::string &name, const glm::vec4 &value)
{
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }
}

void Shader::setUniformMat3(const std::string &name, const glm::mat3 &value)
{
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
    }
}

void Shader::setUniformMat4(const std::string &name, const glm::mat4 &value)
{
    GLint location = getUniformLocation(name);
    if (location != -1)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
    }
}

} // namespace Airwave
