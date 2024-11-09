#include "scene.hpp"
#include "aw_entity.hpp"
#include "system/aw_system.hpp"

namespace Airwave
{
Scene::Scene(const SceneSpecification &specification) : m_specification(specification)
{
    m_registry = std::make_shared<entt::registry>();

}

Scene::~Scene()
{
    destroyAllEntities();
    m_entities.clear();
    m_systems.clear();
    m_entities.clear();
}

std::shared_ptr<AwEntity> Scene::createEntity(const std::string &name)
{
    if (m_rootEntity == nullptr)
    {
        auto entity        = m_registry->create();
        auto specification = AwEntitySpecification(m_specification.name);
        m_rootEntity       = std::make_shared<AwEntity>(shared_from_this(), entity, specification);
    }
    auto entity        = m_registry->create();
    auto specification = AwEntitySpecification(name);
    auto awEntity      = std::make_shared<AwEntity>(shared_from_this(), entity, specification);
    m_entities[entity] = awEntity;
    awEntity->setParent(m_rootEntity);
    return awEntity;
}

std::shared_ptr<AwEntity> Scene::getEntity(entt::entity entity)
{
    auto it = m_entities.find(entity);
    if (it != m_entities.end()) return it->second;
    return nullptr;
}

std::shared_ptr<AwEntity> Scene::getEntity(const UUID &uuid)
{
    for (auto &pair : m_entities)
    {
        if (pair.second->getUUID() == uuid) return pair.second;
    }
    return nullptr;
}

std::shared_ptr<AwEntity> Scene::getEntity(const std::string &name)
{
    for (auto &pair : m_entities)
    {
        if (pair.second->getName() == name) return pair.second;
    }
    AW_ASSERT(false, "Entity " + name + "not found");
    return nullptr;
}

bool Scene::destroyEntity(entt::entity entity)
{
    for (auto &pair : m_entities)
    {
        if (pair.first == entity)
        {
            m_registry->destroy(entity);
            m_entities.erase(entity);
            return true;
        }
    }

    return false;
}

bool Scene::destroyEntity(const UUID &uuid)
{
    for (auto &pair : m_entities)
    {
        if (pair.second->getUUID() == uuid)
        {
            m_registry->destroy(pair.first);
            m_entities.erase(pair.first);
            return true;
        }
    }

    return false;
}

bool Scene::destroyEntity(const std::string &name)
{
    for (auto &pair : m_entities)
    {
        if (pair.second->getName() == name)
        {
            m_registry->destroy(pair.first);
            m_entities.erase(pair.first);
            return true;
        }
    }

    return false;
}

bool Scene::destroyEntity(std::shared_ptr<AwEntity> entity)
{
    return destroyEntity(entity->getEntity());
}

bool Scene::destroyAllEntities()
{
    for (auto &pair : m_entities)
    {
        m_registry->destroy(pair.first);
    }

    m_entities.clear();
    return true;
}

void Scene::addSystem(const std::shared_ptr<AwSystem> &system)
{
    if (system)
    {
        auto &systemRef           = *system;
        std::type_index typeIndex = std::type_index(typeid(systemRef));
        m_systems[typeIndex]      = system;
    }
}

void Scene::removeSystem(const std::shared_ptr<AwSystem> &system)
{
    if (system)
    {
        auto &systemRef           = *system;
        std::type_index typeIndex = std::type_index(typeid(systemRef));
        m_systems.erase(typeIndex);
    }
}

void Scene::updateSystems(float deltaTime)
{
    for (auto &pair : m_systems)
    {
        pair.second->onUpdate(deltaTime, shared_from_this());
    }
}

} // namespace Airwave
