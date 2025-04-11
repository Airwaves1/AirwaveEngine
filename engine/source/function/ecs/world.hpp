#pragma once

#include <entt/entt.hpp>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <map>
#include "core/uuid/uuid.hpp"
#include "core/log/log.hpp"
#include "function/ecs/system.hpp"

namespace Airwave
{
class Serializer;
class World final : public std::enable_shared_from_this<World>
{
  public:
    World();
    ~World();

    // 获取注册表
    entt::registry &getRegistry() { return m_registry; }

    // template<typename T> void registerComponent()
    // {
    //     m_serializer->registerComponent<T>();
    // }

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
    entt::entity getAdminEntity() const { return m_adminEntity; }
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

    // 添加单例组件
    template <typename T, typename... Args> T &addSingletonComponent(Args &&...args)
    {
        addComponent<T>(m_adminEntity, std::forward<Args>(args)...);
        return m_registry.get<T>(m_adminEntity);
    }

    template <typename T> void removeSingletonComponent() { m_registry.remove<T>(m_adminEntity); }
    template <typename T> T &getSingletonComponent() { return m_registry.get<T>(m_adminEntity); }
    template <typename T> bool hasSingletonComponent() const { return m_registry.all_of<T>(m_adminEntity); }
    template <typename T> T *tryGetSingletonComponent() { return m_registry.try_get<T>(m_adminEntity); }

    // 层级操作
    void setParent(entt::entity child, entt::entity parent);
    void addChild(entt::entity parent, entt::entity child);
    void removeChild(entt::entity parent, entt::entity child);
    void removeFromParent(entt::entity child);
    const std::vector<entt::entity> &getChildren(entt::entity entity) const;
    entt::entity getParent(entt::entity entity) const;

    void traverseHierarchy(entt::entity entity, std::function<void(entt::entity)> func, bool include_self = true);
    void traverseHierarchyUp(entt::entity entity, std::function<void(entt::entity)> func, bool include_self = true);

    // 广度优先遍历
    void traverseHierarchyBFS(entt::entity entity, std::function<void(entt::entity)> func, bool include_self = true);

    void printHierarchy(entt::entity entity, int depth = 0, bool include_self = true) const;

    // 系统操作
    void update(float deltaTime);

    template <typename T, typename... Args> void registerSystem(int priority, Args &&...args)
    {
        static_assert(std::is_base_of<System, T>::value, "T must be derived from System");

        if (m_systems.contains(typeid(T)))
        {
            LOG_WARN("System {} already registered", typeid(T).name());
            return;
        }

        auto system          = std::make_unique<T>(std::forward<Args>(args)...);
        system->m_priority   = priority;
        m_systems[typeid(T)] = std::move(system); // 注册系统

        m_systemsByPriority.insert({priority, m_systems[typeid(T)].get()}); // 按优先级插入
    }
    template <typename T> void setSystemPriority(int priority)
    {
        auto it = m_systems.find(typeid(T));
        if (it != m_systems.end())
        {
            auto *system = it->second.get();
            auto range   = m_systemsByPriority.equal_range(system->priority());
            for (auto iter = range.first; iter != range.second; ++iter)
            {
                if (iter->second->priority() == system->priority())
                {
                    m_systemsByPriority.erase(iter);
                    break;
                }
            }
            system->m_priority = priority;
            m_systemsByPriority.insert({priority, system});
        }
        else
        {
            LOG_WARN("System {} not registered", typeid(T).name());
        }
    }
    template <typename T> bool hasSystem() const { return m_systems.find(typeid(T)) != m_systems.end(); }
    template <typename T> void removeSystem()
    {
        auto it = m_systems.find(typeid(T));
        if (it != m_systems.end())
        {
            auto *system = it->second.get();
            auto range   = m_systemsByPriority.equal_range(system->priority());
            for (auto iter = range.first; iter != range.second; ++iter)
            {
                if (iter->second == system)
                {
                    m_systemsByPriority.erase(iter);
                    break;
                }
            }
            m_systems.erase(it);
        }
        else
        {
            LOG_WARN("System {} not registered", typeid(T).name());
        }
    }

    // 场景操作
    void saveScene(const std::string &scene_name);
    void loadScene(const std::string &scene_name);

  private:
    // 检查是否形成祖先环
    bool isAncestor(entt::entity entity, entt::entity potentialAncestor) const;

    entt::registry m_registry;
    entt::entity m_adminEntity               = entt::null;
    std::shared_ptr<Serializer> m_serializer = nullptr; // 序列化器

    // 系统
    std::unordered_map<std::type_index, std::unique_ptr<System>> m_systems; // 系统列表
    std::multimap<int, System *> m_systemsByPriority;                       // 按优先级排序的系统列表

    friend class Serializer;
};
} // namespace Airwave
