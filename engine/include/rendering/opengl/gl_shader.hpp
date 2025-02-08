#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

namespace Airwave
{
class Shader
{
  public:
    Shader(const std::string &vertexSrc, const std::string &fragmentSrc, bool fromFile = true);

    ~Shader();

    void bind() const;
    void unbind() const;

    uint32_t compile(const std::string &source, GLenum shaderType);
    uint32_t link(const uint32_t &vertexShader, const uint32_t &fragmentShader);

    uint32_t getHandle() const { return m_handle; }

  public:
    uint32_t getUniformLocation(const std::string &name);
    void setUniformBool(const std::string &name, bool value);
    void setUniformInt(const std::string &name, int value);
    void setUniformFloat(const std::string &name, float value);
    void setUniformFloat2(const std::string &name, const glm::vec2 &value);
    void setUniformFloat3(const std::string &name, const glm::vec3 &value);
    void setUniformFloat4(const std::string &name, const glm::vec4 &value);
    void setUniformMat3(const std::string &name, const glm::mat3 &value);
    void setUniformMat4(const std::string &name, const glm::mat4 &value);

    void addDefine(const std::string &name, const std::string &value = "");
    void setDefineValue(const std::string &name, const std::string &value);
    void removeDefine(const std::string &name);

  private:
    uint32_t m_handle;

    std::unordered_map<std::string, std::string> m_defines;

    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;

    std::unordered_map<std::string, int> m_uniformLocationCache;
    std::unordered_map<std::string, bool> m_uniformErrordCache;
};
} // namespace Airwave
