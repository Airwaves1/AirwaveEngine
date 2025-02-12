#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

#include "core/log.hpp"
#include "resource/resource.hpp"

namespace Airwave
{
#define RES ResourceManager::GetInstance()
/**
 * @brief 资源管理器
 * 资源以路径为唯一标识符, 通过路径加载资源, 如果资源已经加载, 直接返回资源, 否则加载资源并返回
 */
class ResourceManager
{
  public:
    static ResourceManager &GetInstance()
    {
        static ResourceManager instance;
        return instance;
    }

    using ResourceMap = std::unordered_map<std::string, std::shared_ptr<Resource>>;

    std::unordered_map<std::type_index, ResourceMap> m_resources;

  public:
    template <typename T> std::shared_ptr<T> load(const std::string &path, std::any &&params = std::any())
    {
        const auto type = std::type_index(typeid(T));

        auto &type_map = m_resources[type];

        if (type_map.find(path) != type_map.end())
        {
            return std::static_pointer_cast<T>(type_map[path]);
        }

        auto resource = std::make_shared<T>();
        if (resource->load(std::string(PROJECT_ROOT_DIR)+ "/assets/" + path, std::move(params)))
        {
            type_map[path] = resource;
            return resource;
        }

        LOG_ERROR("Failed to load resource: {0}", path);
        return std::shared_ptr<T>();
    }

    template <typename T> std::shared_ptr<T> get(const std::string &path)
    {
        const auto type = std::type_index(typeid(T));

        auto &type_map = m_resources[type];

        if (type_map.find(path) != type_map.end())
        {
            return std::static_pointer_cast<T>(type_map[path]);
        }

        LOG_ERROR("Resource not found: {0}", path);
        return nullptr;
    }

    template <typename T> bool add(const std::string &path, std::shared_ptr<T> resource)
    {
        const auto type = std::type_index(typeid(T));

        auto &type_map = m_resources[type];

        if (type_map.find(path) != type_map.end())
        {
            LOG_ERROR("Resource already exists: {0}", path);
            return false;
        }

        type_map[path] = resource;
        return true;
    }
};

}; // namespace Airwave