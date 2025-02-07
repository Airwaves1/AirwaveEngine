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
    std::shared_ptr<Texture> loadTexture(const std::string &path, TextureSpecification& spec);

    // 异步加载
    void loadShaderAsync(const std::string &name, const std::string &vertexPath,
                         const std::string &fragmentPath, LoadCallback callback);
    void loadTextureAsync(const std::string &path, TextureSpecification& spec, LoadCallback callback);

  private:
    ResourceManager() = default;

    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaderCache;
    std::unordered_map<std::string, std::shared_ptr<Texture>> m_textureCache;

    std::mutex m_shaderMutex;
    std::mutex m_textureMutex;

    std::vector<std::future<void>> m_futures;
};


} // namespace Airwave
