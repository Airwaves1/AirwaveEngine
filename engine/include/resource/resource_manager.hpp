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
#include <typeindex>
namespace Airwave
{

#define RES ResourceManager::GetInstance()

// 需要加载的资源类型必须实现这个接口
template <typename T, typename... Args>
concept LoadableResource = requires(T t, Args... args) {
    { t.load(args...) } -> std::same_as<bool>;
    { t.reload(args...) } -> std::same_as<bool>;
};

class ResourceManager
{
  private:
    // 使用复合键：类型信息+资源名称
    struct ResourceKey
    {
        std::type_index type;
        std::string name;

        bool operator==(const ResourceKey &other) const { return type == other.type && name == other.name; }
    };

    // 哈希特化
    struct ResourceKeyHash
    {
        size_t operator()(const ResourceKey &key) const
        {
            return std::hash<std::string>{}(key.name) ^ (std::hash<size_t>{}(key.type.hash_code()) << 1);
        }
    };

    std::unordered_map<ResourceKey, std::shared_ptr<Resource>, ResourceKeyHash> m_resources;

    // GPU句柄反向映射
    std::unordered_map<uint32_t, std::weak_ptr<Resource>> m_gpuHandles;


  public:
    static ResourceManager &GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }
    ~ResourceManager() = default;

    template <typename T, typename KeyType, typename... Args>
        requires LoadableResource<T, Args...>
    std::shared_ptr<T> load(KeyType &&key, Args &&...args)
    {
        std::string strKey = std::forward<KeyType>(key);
        ResourceKey mapKey{typeid(T), strKey};

        if (auto it = m_resources.find(mapKey); it != m_resources.end())
        {
            return std::static_pointer_cast<T>(it->second);
        }

        auto res = std::make_shared<T>();
        if (res->load(std::forward<Args>(args)...))
        {
            m_resources[mapKey] = res;
            if (res->m_handle != 0)
            {
                m_gpuHandles[res->m_handle] = res;
            }

            return res;
        }
        LOG_ERROR("Failed to load resource: {0}", strKey);
        return nullptr;
    }

    template <typename T> bool add(const std::string &key, std::shared_ptr<T> resource)
    {
        ResourceKey mapKey{typeid(T), key};
        if (m_resources.find(mapKey) == m_resources.end())
        {
            m_resources[mapKey] = resource;
            if (resource->m_handle != 0)
            {
                m_gpuHandles[resource->m_handle] = resource;
            }
            return true;
        }
        return false;
    }

    template <typename T> std::shared_ptr<T> get(uint32_t handle)
    {
        if (auto it = m_gpuHandles.find(handle); it != m_gpuHandles.end())
        {
            return std::static_pointer_cast<T>(it->second.lock());
        }
        return nullptr;
    }

    template <typename T> std::shared_ptr<T> get(const std::string &key)
    {
        ResourceKey mapKey{typeid(T), key};
        if (auto it = m_resources.find(mapKey); it != m_resources.end())
        {
            return std::static_pointer_cast<T>(it->second);
        }

        LOG_ERROR("Resource not found: {0}", key);

        return nullptr;
    }

    // 安全删除接口
    template <typename T> bool unload(const std::string &key)
    {
        ResourceKey mapKey{typeid(T), key};
        if (auto it = m_resources.find(mapKey); it != m_resources.end())
        {
            if (it->second.use_count() == 1)
            {
                m_resources.erase(it);
                return true;
            }
            else
            {
                LOG_ERROR("Resource still in use: {0}", key);
            }
        }

        return false;
    }

    //获取所有同类资源
    template <typename T> std::vector<std::shared_ptr<T>> getAllOfType()
    {
        std::vector<std::shared_ptr<T>> result;
        ResourceKey mapKey{typeid(T), ""};

        for (auto &[key, res] : m_resources)
        {
            if (key.type == mapKey.type)
            {
                if (auto ptr = std::dynamic_pointer_cast<T>(res))
                {
                    result.push_back(ptr);
                }
            }
        }
        return result;
    }

  private:
    ResourceManager() = default;
};

} // namespace Airwave
