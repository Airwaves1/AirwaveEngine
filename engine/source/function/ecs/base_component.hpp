#pragma once

#include "core/uuid/uuid.hpp"
#include <nlohmann/json.hpp>
#include <entt/entt.hpp>
#include <string>
#include <vector>

namespace Airwave
{

struct UUIDComponent
{
  public:
    constexpr static const char *getComponentName() { return "UUIDComponent"; }
    UUID uuid = UUID::Generate();

    nlohmann::json to_json() const { return nlohmann::json{{"uuid", uuid.toString()}}; }
    void from_json(const nlohmann::json &j)
    {
        if (j.contains("uuid"))
        {
            uuid.fromString(j["uuid"]);
        }
        else
        {
            uuid = UUID::Generate();
        }
    }
};

struct TagComponent
{
  public:
    constexpr static const char *getComponentName() { return "TagComponent"; }
    std::string tag       = "none";
    std::string name      = "entity";
    std::string scene_tag = "none";
    bool is_active        = true;

    nlohmann::json to_json() const { return nlohmann::json{{"tag", tag}, {"name", name}, {"is_active", is_active}}; }
    void from_json(const nlohmann::json &j)
    {   
        tag       = j.value("tag", "none");
        name      = j.value("name", "entity");
        is_active = j.value("is_active", true);
    }
};

struct HierarchyComponent
{
  public:
    constexpr static const char *getComponentName() { return "HierarchyComponent"; }
    entt::entity parent = entt::null;
    std::vector<entt::entity> children;

    UUID parent_uuid = UUID::Invalid(); // 用于存储父实体的UUID
    std::vector<UUID> children_uuids;   // 用于存储子实体的UUIDs

    nlohmann::json to_json(const entt::registry &registry) const
    {
        nlohmann::json j;
        if (parent != entt::null)
        {
            j["parent"] = registry.get<UUIDComponent>(parent).uuid.toString();
        }
        else
        {
            j["parent"] = nullptr;
        }

        j["children"] = nlohmann::json::array();
        for (const auto &child : children)
        {
            j["children"].push_back(registry.get<UUIDComponent>(child).uuid.toString());
        }
        return j;
    }

    // 需延迟处理UUID到实体的转换
    void from_json(const nlohmann::json &j, entt::registry &registry)
    {
        if (j.contains("parent") && !j["parent"].is_null())
        {
            std::string parent_uuid_str = j["parent"];
            parent_uuid.fromString(parent_uuid_str);
        }
        else
        {
            parent_uuid = UUID::Invalid(); // 设置为无效UUID
        }

        if (j.contains("children"))
        {
            children.clear();
            for (const auto &child_uuid_str : j["children"])
            {
                UUID child_uuid;
                child_uuid.fromString(child_uuid_str);
                children_uuids.push_back(child_uuid);
            }
        }
    }
};

} // namespace Airwave
