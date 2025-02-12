#pragma once

#include <unordered_map>
#include <glad/glad.h>

#include "resource/resource.hpp"

#include "core/common.hpp"
#include "rendering/shader.hpp"

namespace Airwave
{
class ShaderResource : public Resource
{
  public:
    ShaderResource() = default;
    ~ShaderResource() override {}

    std::shared_ptr<Shader> getShader() const
    {
        if (m_shader == nullptr)
        {
            return Shader::create_default_shader();
        }
        return m_shader;
    }

  protected:
    bool onLoad(const std::string &path, const std::any &params) override;

  private:
    std::shared_ptr<Shader> m_shader = nullptr;
};

} // namespace Airwave
