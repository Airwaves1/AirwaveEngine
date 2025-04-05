#include "shader.hpp"
#include "platform/file/file_utils.hpp"
#include "core/log/log.hpp"
#include "shader_preprocessor.hpp"

namespace Airwave
{
Uniform Shader::m_uniforms;

Shader::Shader(const std::string &resource_path) : IGLResource(GL_SHADER, resource_path)
{
    // Constructor implementation
}
Shader::~Shader() { release(); }
void Shader::load()
{
    std::string source_code = FileUtils::readFile(m_resource_path);
    if (source_code.empty())
    {
        LOG_ERROR("Failed to load shader source code from: {}", m_resource_path);
        m_state = ResourceState::Error;
        return;
    }

    // Preprocess the shader source code
    auto shaders = ShaderPreprocessor::getInstance().splitShader(source_code);
    if (shaders.empty())
    {
        LOG_ERROR("Failed to split shader source code: {}", m_resource_path);
        m_state = ResourceState::Error;
        return;
    }

    std::vector<GLuint> shader_hadnles;

    for (const auto &shader : shaders)
    {
        ShaderType type = ShaderType::VERTEX; // Default to vertex shader
        if (shader.first == "vertex")
            type = ShaderType::VERTEX;
        else if (shader.first == "fragment")
            type = ShaderType::FRAGMENT;
        else if (shader.first == "geometry")
            type = ShaderType::GEOMETRY;
        else if (shader.first == "compute")
            type = ShaderType::COMPUTE;
        else
        {
            LOG_ERROR("Unknown shader type: {}", shader.first);
            m_state = ResourceState::Error;
            return;
        }

        auto processed_source = ShaderPreprocessor::getInstance().preProcess(shader.second);
        GLuint shader_id      = compileShader(type, processed_source);
        if (shader_id == 0)
        {
            LOG_ERROR("Failed to compile shader: {}", shader.first);
            LOG_ERROR("Shader source: {}", processed_source);
            m_state = ResourceState::Error;
            return;
        }
        shader_hadnles.push_back(shader_id);
    }

    m_handle = linkProgram(shader_hadnles);
    for (const auto &shader_id : shader_hadnles)
    {
        glDeleteShader(shader_id); // Clean up shader handles after linking
    }
}
void Shader::reload() {}
void Shader::release()
{
    if (m_handle != 0)
    {
        glDeleteProgram(m_handle);
        m_handle = 0;
    }
    m_state = ResourceState::Unloaded;
}
GLuint Shader::compileShader(ShaderType type, const std::string &source)
{
    GLuint shader_id        = glCreateShader(static_cast<GLenum>(type));
    const char *source_cstr = source.c_str();
    glShaderSource(shader_id, 1, &source_cstr, nullptr);
    glCompileShader(shader_id);

    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLint log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> info_log(log_length);
        glGetShaderInfoLog(shader_id, log_length, nullptr, info_log.data());
        LOG_ERROR("Failed to compile shader: {}\n{}", static_cast<int>(type), info_log.data());
        glDeleteShader(shader_id); // Clean up on failure
        return 0;
    }
    return shader_id;
}
GLuint Shader::linkProgram(const std::vector<GLuint> &shaders)
{
    GLuint program_id = glCreateProgram();
    for (const auto &shader_id : shaders)
    {
        glAttachShader(program_id, shader_id);
    }
    glLinkProgram(program_id);

    GLint success;
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        GLint log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &log_length);
        std::vector<char> info_log(log_length);
        glGetProgramInfoLog(program_id, log_length, nullptr, info_log.data());
        LOG_ERROR("Failed to link program: {}", info_log.data());
        glDeleteProgram(program_id); // Clean up on failure
        return 0;
    }
    return program_id;
}
void Shader::bind() const
{
    if (m_handle != 0)
    {
        glUseProgram(m_handle);
    }
    else
    {
        LOG_ERROR("Shader program is not loaded or invalid.");
    }
}
void Shader::unbind() const { glUseProgram(0); }
} // namespace Airwave