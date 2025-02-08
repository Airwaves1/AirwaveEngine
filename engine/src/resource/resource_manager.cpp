#include "resource/resource_manager.hpp"

#include "rendering/opengl/gl_shader.hpp"
#include "rendering/opengl/gl_texture.hpp"
#include "utils/file_utils.hpp"
namespace Airwave
{
// ==================== Shader 加载 ====================
std::shared_ptr<Shader> ResourceManager::loadShader(const std::string &name,
                                                    const std::string &vertexPath,
                                                    const std::string &fragmentPath)
{
    std::lock_guard<std::mutex> lock(m_shaderMutex);
    auto it = m_shaderCache.find(name);
    if (it != m_shaderCache.end())
    {
        return it->second;
    }

    auto shader = std::make_shared<Shader>(vertexPath, fragmentPath);
    if (!shader)
    {
        LOG_ERROR("Failed to load shader '{0}'", name);
        return nullptr;
    }

    m_shaderCache[name] = shader;
    return shader;
}

void ResourceManager::loadShaderAsync(const std::string &name, const std::string &vertexPath,
                                      const std::string &fragmentPath, LoadCallback callback)
{
    std::lock_guard<std::mutex> lock(m_shaderMutex);

    if (m_shaderCache.find(name) != m_shaderCache.end())
    {
        callback(m_shaderCache[name]);
        return;
    }

    m_futures.push_back(std::async(std::launch::async,
                                   [this, name, vertexPath, fragmentPath, callback]()
                                   {
                                       auto shader =
                                           std::make_shared<Shader>(vertexPath, fragmentPath);
                                       if (!shader)
                                       {
                                           LOG_ERROR("Failed to load shader '{0}'", name);
                                           return;
                                       }

                                       {
                                           std::lock_guard<std::mutex> lock(m_shaderMutex);
                                           m_shaderCache[name] = shader;
                                       }

                                       callback(shader);
                                   }));
}

// ==================== 纹理加载 ====================

std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string &path,
                                                      TextureSpecification &spec)
{
    std::lock_guard<std::mutex> lock(m_textureMutex);
    auto it = m_textureCache.find(path);
    if (it != m_textureCache.end())
    {
        return it->second;
    }

    auto texture = std::make_shared<Texture>(path, spec);
    if (!texture)
    {
        LOG_ERROR("Failed to load texture '{0}'", path);
        return nullptr;
    }

    m_textureCache[path] = texture;
    return texture;
}

std::string ResourceManager::loadFile(const std::string &path){
    const std::string& content = FileUtils::ReadFile(path);
    m_fileCache[path] = content;
    return content;
}

void ResourceManager::loadTextureAsync(const std::string &path, TextureSpecification &spec,
                                       LoadCallback callback)
{
    std::lock_guard<std::mutex> lock(m_textureMutex);

    if (m_textureCache.find(path) != m_textureCache.end())
    {
        callback(m_textureCache[path]);
        return;
    }

    m_futures.push_back(std::async(std::launch::async,
                                   [this, path, spec, callback]()
                                   {
                                       auto texture = std::make_shared<Texture>(path, spec);
                                       if (!texture)
                                       {
                                           LOG_ERROR("Failed to load texture '{0}'", path);
                                           return;
                                       }

                                       {
                                           std::lock_guard<std::mutex> lock(m_textureMutex);
                                           m_textureCache[path] = texture;
                                       }

                                       callback(texture);
                                   }));
}

void ResourceManager::addShader(const std::string &name, std::shared_ptr<Shader> shader)
{
    std::lock_guard<std::mutex> lock(m_shaderMutex);
    m_shaderCache[name] = shader;
}
void ResourceManager::addTexture(const std::string &path, std::shared_ptr<Texture> texture)
{
    std::lock_guard<std::mutex> lock(m_textureMutex);
    m_textureCache[path] = texture;
}

} // namespace Airwave
