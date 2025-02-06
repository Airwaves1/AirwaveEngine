#pragma once

#include <entt/entt.hpp>
#include <map>
#include <memory>
#include <type_traits>

#include "core/log.hpp"

namespace Airwave
{
class Application;
class AwEntity;
class AwSystem;

class AwScene
{
  public:
    AwScene(Application *appContext);
    ~AwScene();

    // std::shared_ptr<AwEntity> createEntity(const std::string &name = "Entity");
    AwEntity *createEntity(const std::string &name = "Entity");
    AwEntity *createDefaultEntity(const std::string &name = "Entity", const std::string &tag = "obj"
                                                                                               "ec"
                                                                                               "t");

    AwEntity *getEntity(entt::entity entity);
    AwEntity *getEntity(const std::string &name);

    void traverseEntity(AwEntity *entity, std::function<void(AwEntity *)> callback);

    void destroyEntity(AwEntity &entity);
    void destroyEntity(entt::entity entity);
    void destroyAllEntities();

    bool isValideEntity(AwEntity *entity);
    void printHierarchy(AwEntity *entity, int level);

    void setEntityParent(AwEntity *entity, AwEntity *parent);
    const std::vector<AwEntity *> &getEntityChildren(AwEntity *entity);
    AwEntity *getEntityParent(AwEntity *entity);

    entt::registry &getRegistry() { return m_registry; }
    AwEntity *getAdminEntity() { return m_adminEntity; }

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

    Application *m_appContext{nullptr};

    std::unordered_map<entt::entity, std::unique_ptr<AwEntity>> m_entities;
    std::multimap<int, std::unique_ptr<AwSystem>> m_systems;

    AwEntity *m_adminEntity;
};

} // namespace Airwave
