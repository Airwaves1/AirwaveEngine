#pragma once

#include <entt/entt.hpp>
#include <map>
#include <memory>
#include <type_traits>

#include "utils/utils.hpp"
#include "core/log.hpp"

#include "ecs/aw_system.hpp"

namespace Airwave
{
class Application;
class AwEntity;

class AwScene
{
  public:
    AwScene() = default;
    AwScene(Application *appContext);
    ~AwScene();

    entt::entity createEntity(const std::string &name = "Entity", const std::string &tag = "object");
    entt::entity createDefaultEntity(const std::string &name = "Entity", const std::string &tag = "object");

    entt::entity getEntity(const std::string &name);

    entt::entity cloneEntity(entt::entity entity);

    void traverseEntity(entt::entity entity, std::function<void(entt::entity)> callback);

    void destroyEntity(entt::entity entity);
    void destroyAllEntities();

    template <typename T, typename... Args> T &addComponent(entt::entity entity, Args &&...args)
    {
        if (!m_registry.valid(entity))
        {
            LOG_ERROR("entity is not valid");
            assert(false);
        }

        if (m_registry.all_of<T>(entity))
        {
            LOG_WARN("entity {1} already has component: {0}", static_cast<uint32_t>(entity), demangle(typeid(T).name()));
            return m_registry.get<T>(entity);
        }

        return m_registry.emplace<T>(entity, std::forward<Args>(args)...);
    }

    template <typename T> T &getComponent(entt::entity entity)
    {
        if (!m_registry.valid(entity))
        {
            LOG_ERROR("entity is not valid");
            assert(false);
        }

        if (!m_registry.all_of<T>(entity))
        {
            LOG_ERROR("entity has no component: {0}", demangle(typeid(T).name()));
            assert(false);
        }

        return m_registry.get<T>(entity);
    }

    template <typename T> void removeComponent(entt::entity entity)
    {
        if (!m_registry.valid(entity))
        {
            LOG_ERROR("entity is not valid");
        }

        if (!m_registry.all_of<T>(entity))
        {
            LOG_ERROR("entity has no component: {0} ", demangle(typeid(T).name()));
        }

        m_registry.remove<T>(entity);
    }

    template <typename T> bool hasComponent(entt::entity entity) { return m_registry.all_of<T>(entity); }

    template <typename T> T *tryGetComponent(entt::entity entity) { return m_registry.try_get<T>(entity); }

    template <typename... T> auto getView() { return m_registry.view<T...>(); }


    bool isValideEntity(entt::entity entity) { return m_registry.valid(entity); }

    void printHierarchy(entt::entity entity, int level);

    void setEntityParent(entt::entity child, entt::entity parent = entt::null);

    const std::vector<entt::entity> &getEntityChildren(entt::entity entity);
    entt::entity getEntityParent(entt::entity entity);

    entt::registry &getRegistry() { return m_registry; }
    entt::dispatcher &getDispatcher() { return m_dispatcher; }
    entt::entity getAdminEntity() { return m_adminEntity; }

    // 系统管理
    template <typename T> void addSystem(int order = 0)
    {
        static_assert(std::is_base_of<AwSystem, T>::value, "T must be derived from AwSystem");
        auto system     = std::make_unique<T>();
        system->m_scene = this;
        system->onInit();
        m_systems.insert({order, std::move(system)});
    }

    void update(float deltaTime);

    Application *getApplication();

  private:
    entt::registry m_registry;
    entt::dispatcher m_dispatcher;

    entt::entity m_adminEntity = entt::null;

    Application *m_appContext{nullptr};

    std::multimap<int, std::unique_ptr<AwSystem>> m_systems;
};

} // namespace Airwave
