#pragma once

#include <entt/entt.hpp>
#include "core/uuid/uuid.hpp"
namespace Airwave
{
  class Serializer; 
class World final
{
  public:
    World();
    ~World();

    // 获取注册表
    const entt::registry &getRegistry() { return m_registry; }

    // 实体管理
    entt::entity createEntity(const std::string &name = "entity", const std::string &tag = "none", bool is_active = true);
    entt::entity createDefaultEntity(const std::string &name = "entity", const std::string &tag = "none", bool is_active = true);
    void destroyEntity(entt::entity &entity, bool recursive = true);
    void destroyEntityByUUID(const UUID &uuid);
    void destroyEntityByName(const std::string &name);
    void destroyAllEntities();

    entt::entity cloneEntity(entt::entity source, const std::string &name = "entity", const std::string &tag = "none", bool is_active = true)
    {
        return entt::null; // TODO: 实现深拷贝
    }
    entt::entity deepCloneEntity(entt::entity source, const std::string &name = "entity", const std::string &tag = "none", bool is_active = true,
                                 entt::entity newParent = entt::null)
    {
        return entt::null;
    }

    // 获取实体
    entt::entity getadminEntity() const { return m_adminEntity; }
    entt::entity getEntityByUUID(const UUID &uuid) const;
    entt::entity getEntityByName(const std::string &name) const;

    // 组件操作
    template <typename T, typename... Args> T &addComponent(entt::entity entity, Args &&...args)
    {
        return m_registry.emplace<T>(entity, std::forward<Args>(args)...);
    }
    template <typename T> void removeComponent(entt::entity entity) { m_registry.remove<T>(entity); }
    template <typename T> T &getComponent(entt::entity entity) { return m_registry.get<T>(entity); }
    template <typename T> bool hasComponent(entt::entity entity) const { return m_registry.all_of<T>(entity); }
    template <typename T> T *tryGetComponent(entt::entity entity) { return m_registry.try_get<T>(entity); }

    // 层级操作
    void setParent(entt::entity child, entt::entity parent);
    void addChild(entt::entity parent, entt::entity child);
    void removeChild(entt::entity parent, entt::entity child);
    void removeFromParent(entt::entity child);
    const std::vector<entt::entity> &getChildren(entt::entity entity) const;
    entt::entity getParent(entt::entity entity) const;
    void traverseHierarchy(entt::entity entity, std::function<void(entt::entity)> func, bool include_self = true);

    // 场景操作
    void saveScene(const std::string &scene_name);
    void loadScene(const std::string &scene_name);

  private:
    // 检查是否形成祖先环
    bool isAncestor(entt::entity entity, entt::entity potentialAncestor) const;

    entt::registry m_registry;
    entt::entity m_adminEntity = entt::null;
    std::shared_ptr<Serializer> m_serializer = nullptr; // 序列化器

    friend class Serializer;
};
} // namespace Airwave
