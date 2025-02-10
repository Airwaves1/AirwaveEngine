#pragma once

#include "resource/resource.hpp"
#include "core/common.hpp"
#include <unordered_map>
#include <glad/glad.h>

namespace Airwave
{
class ShaderResourceEvent : public ResourceEvent
{
  public:
    ShaderResourceEvent(uint32_t handle, UUID uuid) : ResourceEvent(handle, uuid) {}
    std::string getType() const override { return "ShaderResourceEvent"; }
};

class ShaderResource : public Resource
{
  public:
    ShaderResource() = default;
    ~ShaderResource() override;

    ResourceType getType() const override { return ResourceType::Shader; }

    bool load(const std::string &vertexPath, const std::string &fragmentPath);

    bool reload(const std::string &vertexPath, const std::string &fragmentPath);

  private:
    uint32_t compile(const std::string &source, GLenum shaderType);
    uint32_t link(const uint32_t &vertexShader, const uint32_t &fragmentShader);

    std::unordered_map<std::string, std::string> m_defines;

    std::string m_vertexShaderSource;
    std::string m_fragmentShaderSource;

    std::string m_vertexPath;
    std::string m_fragmentPath;
};

} // namespace Airwave
