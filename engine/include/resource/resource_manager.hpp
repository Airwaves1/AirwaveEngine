#pragma once

#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include <queue>
#include <future>
#include <variant>
#include <concepts>

#include "core/log.hpp"
#include "resource/resource.hpp"

namespace Airwave
{

#define RES ResourceManager::GetInstance()

class ResourceManager
{
  private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;

  public:
    static ResourceManager &GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }
    ~ResourceManager() = default;

    template <typename T> std::shared_ptr<T> load(const std::string &path, std::any &&params = {})
    {
        if (auto it = m_resources.find(path); it != m_resources.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        auto resource = std::make_shared<T>();
        if (resource->load(PROJECT_ROOT_DIR + path, params))
        {
            m_resources[path] = resource;
            return resource;
        }

        LOG_ERROR("Failed to load resource: {0}", path);
        return nullptr;
    }

    template <typename T> bool add(const std::string &path, std::shared_ptr<T> resource)
    {
        if (auto it = m_resources.find(path); it == m_resources.end())
        {
            m_resources[path] = resource;
            return true;
        }

        LOG_ERROR("Resource already exists: {0}", path);

        return false;
    }

    template <typename T> std::shared_ptr<T> get(const std::string &path)
    {
        if (auto it = m_resources.find(path); it != m_resources.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }
        return nullptr;
    }

    template <typename T> bool unload(const std::string &path)
    {
        if (auto it = m_resources.find(path); it != m_resources.end())
        {
            if (it->second.use_count() == 1)
            {
                m_resources.erase(it);
                return true;
            }
            else
            {
                LOG_ERROR("Resource still in use: {0}", path);
            }
        }
        return false;
    }

  private:
    ResourceManager() = default;
};

} // namespace Airwave
