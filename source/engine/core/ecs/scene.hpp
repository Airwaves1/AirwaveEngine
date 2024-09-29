#pragma once

#include "uuid.hpp"
#include <entt/entity/registry.hpp>
#include <memory>
#include <vector>
#include <unordered_map>
#include "utils/log.hpp"

namespace Airwave
{

class AwEntity;
class AwSystem;

struct SceneSpecification
{
    std::string name = "Scene";
    UUID uuid;

    SceneSpecification() : uuid(UUID::Generate()) {}
    SceneSpecification(const std::string &name) : name(name), uuid(UUID::Generate()) {}
};

class Scene : public std::enable_shared_from_this<Scene>
{
  public:
    Scene(const SceneSpecification &specification);

    ~Scene();

    // 创建实体
    std::shared_ptr<AwEntity> createEntity(const std::string &name = "Entity");

    // 获取实体
    std::shared_ptr<AwEntity> getEntity(entt::entity entity);
    std::shared_ptr<AwEntity> getEntity(const UUID &uuid);
    std::shared_ptr<AwEntity> getEntity(const std::string &name);

    // 销毁实体
    bool destroyEntity(entt::entity entity);
    bool destroyEntity(const UUID &uuid);
    bool destroyEntity(const std::string &name);
    bool destroyEntity(std::shared_ptr<AwEntity> entity);
    bool destroyAllEntities();

    // 获取根实体
    std::shared_ptr<AwEntity> getRootEntity() { return m_rootEntity; }

    // 创建系统
    std::shared_ptr<AwSystem> addSystem(std::shared_ptr<AwSystem> system, std::string name);


    // 获取系统
    std::shared_ptr<AwSystem> getSystem(std::string name);

    // 销毁系统
    bool destroySystem(std::string name);

    // 更新系统
    bool updateSystems(float deltaTime);

    // 获取注册表
    std::shared_ptr<entt::registry> getRegistry() const { return m_registry; }

    // 获取场景规格
    SceneSpecification getSpecification() const { return m_specification; }

    // 设置场景规格
    void setSpecification(const SceneSpecification &specification)
    {
        m_specification = specification;
    }

    // 获取所有实体
    std::unordered_map<entt::entity, std::shared_ptr<AwEntity>> getEntities() { return m_entities; }

    // 获取所有系统
    std::unordered_map<std::string, std::shared_ptr<AwSystem>> getSystems() { return m_systems; }

    // 获取所有实体数量
    size_t getEntityCount() { return m_entities.size(); }

    // 获取所有系统数量
    size_t getSystemCount() { return m_systems.size(); }

  private:
    SceneSpecification m_specification;

    std::shared_ptr<entt::registry> m_registry; // 实体注册表

    std::unordered_map<std::string, std::shared_ptr<AwSystem>> m_systems; // 系统列表

    std::unordered_map<entt::entity, std::shared_ptr<AwEntity>> m_entities; // 实体列表

    std::shared_ptr<AwEntity> m_rootEntity; // 根实体
};

} // namespace Airwave
