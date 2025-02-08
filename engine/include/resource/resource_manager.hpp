#pragma once

#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <queue>
#include <mutex>
#include <future>
#include <variant>

#include "core/log.hpp"

namespace Airwave
{
class Shader;
class Texture;
struct TextureSpecification;

class ResourceManager
{
  public:
    using ResourceVariant = std::variant<std::shared_ptr<Shader>, std::shared_ptr<Texture>>;
    using LoadCallback    = std::function<void(ResourceVariant)>;

    ~ResourceManager()
    {
        for (auto &future : m_futures)
        {
            if (future.valid()) future.wait();
        }
    }

    static ResourceManager &GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    // 同步加载
    std::shared_ptr<Shader> loadShader(const std::string &name, const std::string &vertexPath,
                                       const std::string &fragmentPath);
    std::shared_ptr<Texture> loadTexture(const std::string &path, TextureSpecification &spec);

    std::string loadFile(const std::string &path);

    // 异步加载
    void loadShaderAsync(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath,
                         LoadCallback callback);
    void loadTextureAsync(const std::string &path, TextureSpecification &spec, LoadCallback callback);

    // 添加资源
    void addShader(const std::string &name, std::shared_ptr<Shader> shader);
    void addTexture(const std::string &path, std::shared_ptr<Texture> texture);

    // 获取资源
    std::shared_ptr<Shader> getShader(const std::string &name)
    {
        std::lock_guard<std::mutex> lock(m_shaderMutex);
        auto it = m_shaderCache.find(name);
        if (it != m_shaderCache.end())
        {
            return it->second;
        }
        return nullptr;
    }

    std::shared_ptr<Texture> getTexture(const std::string &path)
    {
        std::lock_guard<std::mutex> lock(m_textureMutex);
        auto it = m_textureCache.find(path);
        if (it != m_textureCache.end())
        {
            return it->second;
        }
        else
        {
            LOG_ERROR("Texture not found: {}", path);
        }
        return nullptr;
    }

  private:
    ResourceManager() = default;

    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaderCache;
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureCache;
    std::unordered_map<std::string, std::string> m_fileCache;

    std::mutex m_shaderMutex;
    std::mutex m_textureMutex;

    std::vector<std::future<void>> m_futures;
};

} // namespace Airwave
