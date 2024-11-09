#include "shader_lib.hpp"

namespace Airwave
{
void ShaderLibrary::add(const std::string &name, const std::shared_ptr<Shader> &shader)
{
    m_shaders[name] = shader;
}

std::shared_ptr<Shader> ShaderLibrary::load(const std::string &name, const std::string &vertexPath,
                                            const std::string &fragmentPath, bool fromFile)
{
    if (m_shaders.find(name) != m_shaders.end())
    {
        return m_shaders[name];
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(vertexPath, fragmentPath, fromFile);
    if (!shader)
    {
        LOG_ERROR("Failed to load shader '{0}'", name);
        return nullptr;
    }

    m_shaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> ShaderLibrary::load(const std::string &name, const std::string &filePath)
{
    // TODO: Implement this function
    return nullptr;
}

std::shared_ptr<Shader> ShaderLibrary::get(const std::string &name)
{
    if (m_shaders.find(name) == m_shaders.end())
    {
        LOG_WARN("Shader '{0}' not found", name);
        return nullptr;
    }

    return m_shaders[name];
}

void ShaderLibrary::remove(const std::string &name)
{
    auto it = m_shaders.find(name);
    if (it != m_shaders.end())
    {
        m_shaders.erase(it);
        LOG_INFO("Shader '{0}' removed", name);
    }
    else
    {
        LOG_WARN("Shader '{0}' not found for removal", name);
    }
}

} // namespace Airwave