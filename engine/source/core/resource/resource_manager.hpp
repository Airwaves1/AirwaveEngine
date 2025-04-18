#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <typeindex>
#include <type_traits>
#include <mutex>

#include "core/resource/resource.hpp"
namespace Airwave
{

class ResourceManager
{
  public:
    template <typename ResourceType, typename... Args> std::shared_ptr<ResourceType> loadResource(const std::string &path, Args &&...args)
    {
        static_assert(std::is_base_of<IResource, ResourceType>::value, "ResourceType must derive from IResource");

        std::lock_guard<std::mutex> lock(m_mutex);

        auto key = getKey<ResourceType>(path);

        auto it = m_resources.find(key);
        if (it != m_resources.end())
        {
            return std::static_pointer_cast<ResourceType>(it->second);
        }

        auto resource = std::make_shared<ResourceType>(path, std::forward<Args>(args)...);
        resource->load();

        m_resources[key] = resource;
        return resource;
    }

    template <typename ResourceType> std::shared_ptr<ResourceType> getResource(const std::string &path)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto key = getKey<ResourceType>(path);
        auto it  = m_resources.find(key);
        if (it != m_resources.end())
        {
            return std::static_pointer_cast<ResourceType>(it->second);
        }
        return nullptr;
    }

    template <typename ResourceType> void reloadResource(const std::string &path)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto key = getKey<ResourceType>(path);
        auto it  = m_resources.find(key);
        if (it != m_resources.end())
        {
            it->second->reload();
        }
    }

    template <typename ResourceType> void releaseResource(const std::string &path)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        auto key = getKey<ResourceType>(path);
        auto it  = m_resources.find(key);
        if (it != m_resources.end())
        {
            it->second->release();
            m_resources.erase(it);
        }
    }

    void releaseAll()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto &[_, resource] : m_resources)
        {
            resource->release();
        }
        m_resources.clear();
    }

  private:
    std::unordered_map<std::string, std::shared_ptr<IResource>> m_resources;
    std::mutex m_mutex;

    template <typename ResourceType> std::string getKey(const std::string &path) { return std::string(typeid(ResourceType).name()) + ":" + path; }
};

} // namespace Airwave
