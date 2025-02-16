#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <memory>

namespace Airwave
{
class Shader
{
  public:
    static std::shared_ptr<Shader> create_default_shader();

    Shader(const std::string &vertexSrc, const std::string &fragmentSrc);
    ~Shader();

    void bind() const { glUseProgram(m_handle); }
    void unbind() const { glUseProgram(0); }

    uint32_t getHandle() const { return m_handle; }

    uint32_t compile(const std::string &source, GLenum shaderType);
    uint32_t link(const uint32_t &vertexShader, const uint32_t &fragmentShader);

    // 添加宏定义
    void add_defines(std::vector<std::pair<std::string, std::string>> defines);

    std::string getVertexSrc() const { return m_vertexSrc; }
    std::string getFragmentSrc() const { return m_fragmentSrc; }

  private:
    uint32_t m_handle;

    std::string m_vertexSrc;
    std::string m_fragmentSrc;

    std::unordered_map<std::string, std::string> m_defines;
};
} // namespace Airwave
