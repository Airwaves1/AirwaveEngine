#pragma once

#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include <type_traits>
#include <fstream>
#include "core/log/log.hpp"
#include "core/uuid/uuid.hpp"
#include "base_component.hpp"
#include "world.hpp"

namespace detail
{
// 检测to_json是否需要registry参数
template <typename T> struct has_to_json_with_registry
{
    template <typename U> static auto test(int) -> decltype(std::declval<U>().to_json(std::declval<const entt::registry &>()), std::true_type{});

    template <typename U> static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
};

// 检测from_json是否需要registry参数
template <typename T> struct has_from_json_with_registry
{
    template <typename U>
    static auto test(int)
        -> decltype(std::declval<U>().from_json(std::declval<const nlohmann::json &>(), std::declval<entt::registry &>()), std::true_type{});

    template <typename U> static auto test(...) -> std::false_type;

    static constexpr bool value = decltype(test<T>(0))::value;
};
} // namespace detail

namespace Airwave
{

class Serializer
{
  public:
    using SerializeFunc   = std::function<void(const entt::registry &, entt::entity, nlohmann::json &)>;
    using DeserializeFunc = std::function<void(entt::registry &, entt::entity, const nlohmann::json &)>;

    template <typename Component> void registerComponent()
    {
        const auto typeHash = entt::type_hash<Component>::value();

        // 序列化函数
        serializeFunctions_[typeHash] = [](const entt::registry &registry, entt::entity entity, nlohmann::json &json)
        {
            if constexpr (detail::has_to_json_with_registry<Component>::value)
            {
                if (registry.all_of<Component>(entity))
                {
                    const auto &comp                    = registry.get<Component>(entity);
                    json[Component::getComponentName()] = comp.to_json(registry);
                }
            }
            else
            {
                if (registry.all_of<Component>(entity))
                {
                    const auto &comp                    = registry.get<Component>(entity);
                    json[Component::getComponentName()] = comp.to_json();
                }
            }
        };

        // 反序列化函数
        deserializeFunctions_[typeHash] = [](entt::registry &registry, entt::entity entity, const nlohmann::json &json)
        {
            if (json.contains(Component::getComponentName()))
            {
                auto &comp = registry.emplace_or_replace<Component>(entity);

                if constexpr (detail::has_from_json_with_registry<Component>::value)
                {
                    comp.from_json(json[Component::getComponentName()], registry);
                }
                else
                {
                    comp.from_json(json[Component::getComponentName()]);
                }
            }
        };
    }

    // 序列化单个实体
    nlohmann::json serializeEntity(const entt::registry &registry, entt::entity entity)
    {
        nlohmann::json entityJson;

        // 必须包含UUID组件
        if (!registry.all_of<UUIDComponent>(entity))
        {
            throw std::runtime_error("Entity must have UUIDComponent for serialization");
        }

        const auto &uuid   = registry.get<UUIDComponent>(entity).uuid;
        entityJson["uuid"] = uuid.toString();

        // 序列化所有注册的组件
        for (const auto &[type, func] : serializeFunctions_)
        {
            func(registry, entity, entityJson);
        }

        return entityJson;
    }

    // 反序列化实体（返回反序列化后的实体）
    entt::entity deserializeEntity(entt::registry &registry, const nlohmann::json &entityJson,
                                   std::unordered_map<std::string, entt::entity> &uuidToEntityMap)

    {
        // 创建实体并添加UUID组件
        auto entity = registry.create();

        // 反序列化所有注册的组件
        for (const auto &[type, func] : deserializeFunctions_)
        {
            func(registry, entity, entityJson);
        }

        auto &uuidComponent       = registry.get<UUIDComponent>(entity);
        auto uuid_str             = uuidComponent.uuid.toString();
        uuidToEntityMap[uuid_str] = entity; // 将UUID映射到实体

        return entity;
    }

    // 序列化整个场景
    nlohmann::json serializeScene(const entt::registry &registry, const std::string &sceneName)
    {
        nlohmann::json sceneJson;
        sceneJson["scene_name"] = sceneName;

        auto view = registry.view<UUIDComponent>();
        for (auto entity : view)
        {
            auto entityJson = serializeEntity(registry, entity);
            sceneJson["entities"].push_back(entityJson);
        }

        return sceneJson;
    }

    // 反序列化整个场景
    void DeserializeScene(entt::registry &registry, World *world, const nlohmann::json &sceneJson)
    {
        registry.clear(); // 注意：清空现有场景

        std::unordered_map<std::string, entt::entity> uuidToEntityMap;

        for (const auto &entityJson : sceneJson["entities"])
        {
            deserializeEntity(registry, entityJson, uuidToEntityMap);
        }

        // 处理层级组件, 将uuid映射到实体
        // 获取层级组件的实体视图
        auto view            = registry.view<HierarchyComponent, TagComponent>();
        entt::entity tmpRoot = entt::null;
        for (auto entity : view)
        {
            auto &hierarchy = view.get<HierarchyComponent>(entity);
            auto &tag       = view.get<TagComponent>(entity);

            if (tag.name == "adminEntity")
            {
                tmpRoot = entity; // 找到根实体
            }

            auto it = uuidToEntityMap.find(hierarchy.parent_uuid.toString());
            if (it != uuidToEntityMap.end())
            {
                auto &parent_hierarchy = registry.get<HierarchyComponent>(it->second);
                auto &parent_tag       = registry.get<TagComponent>(it->second);

                hierarchy.parent = it->second;               // 设置父实体
                parent_hierarchy.children.push_back(entity); // 添加子实体到父实体
            }

            hierarchy.children_uuids.clear(); // 清空UUID列表
        }

        // 处理根实体的子实体
        if (registry.valid(world->m_adminEntity))
        {
            registry.destroy(world->m_adminEntity); // 销毁原有的根实体
        }
        world->m_adminEntity = tmpRoot;         // 设置新的根实体
    }

    static void writeToFile(const std::string &filePath, const nlohmann::json &json)
    {
        std::ofstream outFile(filePath);
        if (outFile.is_open())
        {
            outFile << json.dump(4); // 4个空格缩进
            outFile.close();
            LOG_INFO("Scene saved to: {}", filePath);
        }
        else
        {
            LOG_ERROR("Failed to open file for writing: {}", filePath);
        }
    }

    static nlohmann::json readFromFile(const std::string &filePath)
    {
        std::ifstream inFile(filePath);
        if (inFile.is_open())
        {
            nlohmann::json json;
            inFile >> json;
            inFile.close();
            return json;
        }
        else
        {
            LOG_ERROR("Failed to open file for reading: {}", filePath);
            return nullptr; // 返回空的json对象
        }
    }

  private:
    std::unordered_map<entt::id_type, SerializeFunc> serializeFunctions_;
    std::unordered_map<entt::id_type, DeserializeFunc> deserializeFunctions_;
};

} // namespace Airwave