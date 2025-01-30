#include "ecs/aw_scene.hpp"
#include "ecs/aw_system.hpp"
#include "ecs/aw_entity.hpp"

#include "ecs/components/hierarchy_component.hpp"
#include "ecs/components/tag_component.hpp"
#include "ecs/components/transform_component.hpp"

#include "core/application.hpp"

namespace Airwave
{
AwScene::AwScene(Application *appContext) : m_appContext(appContext)
{
    m_adminEntity = createEntity("AdminEntity");
    m_adminEntity->addComponent<TagComponent>("AdminEntity");
    m_adminEntity->addComponent<HierarchyComponent>();
}
AwScene::~AwScene() {}

AwEntity *AwScene::createEntity(const std::string &name)
{
    auto entity        = m_registry.create();
    m_entities[entity] = std::make_unique<AwEntity>(this, entity, name);
    return m_entities[entity].get();
}
AwEntity *AwScene::createDefaultEntity(const std::string &name, const std::string &tag)
{
    auto entity = createEntity(name);
    entity->addComponent<TagComponent>(name, tag);
    entity->addComponent<TransformComponent>();
    auto &hierachy = entity->addComponent<HierarchyComponent>();
    setEntityParent(entity, m_adminEntity);
    return entity;
}

AwEntity *AwScene::getEntity(entt::entity entity)
{
    if (m_entities.find(entity) != m_entities.end())
    {
        return m_entities[entity].get();
    }
    return nullptr;
}

void AwScene::traverseEntity(AwEntity *entity, std::function<void(AwEntity *)> callback)
{
    callback(entity);
    auto &children = entity->getComponent<HierarchyComponent>().children;
    for (auto &child : children)
    {
        traverseEntity(child, callback);
    }
}
void AwScene::destroyEntity(AwEntity &entity)
{
    m_registry.destroy(entity.m_entity);
    m_entities.erase(entity.m_entity);
}
void AwScene::destroyEntity(entt::entity entity)
{
    m_registry.destroy(entity);
    m_entities.erase(entity);
}
void AwScene::destroyAllEntities()
{
    m_registry.clear();
    m_entities.clear();
}

bool AwScene::isValideEntity(AwEntity *entity)
{
    if (entity == nullptr)
    {
        return false;
    }
    return m_registry.valid(entity->m_entity);
}

void AwScene::setEntityParent(AwEntity *entity, AwEntity *parent)
{
    if (!isValideEntity(entity) && !isValideEntity(parent))
    {
        throw std::invalid_argument("entity or parent is invalid");
    }

    // 解除原来的父子关系
    auto &oldHierarchy = entity->getComponent<HierarchyComponent>();
    if (isValideEntity(oldHierarchy.parent))
    {
        auto &oldParentHierarchy = oldHierarchy.parent->getComponent<HierarchyComponent>();
        auto &children           = oldParentHierarchy.children;
        children.erase(std::remove(children.begin(), children.end(), entity), children.end());
        oldHierarchy.parent = nullptr;
    }

    // 设置新的父子关系
    oldHierarchy.parent = parent;
    auto &newHierarchy  = parent->getComponent<HierarchyComponent>();
    newHierarchy.children.push_back(entity);
}

void AwScene::printHierarchy(AwEntity *entity, int level)
{
    for (int i = 0; i < level; i++)
    {
        std::cout << "  ";
    }
    std::cout << entity->getComponent<TagComponent>().name << std::endl;

    auto &children = entity->getComponent<HierarchyComponent>().children;
    for (auto &child : children)
    {
        printHierarchy(child, level + 1);
    }
}

const std::vector<AwEntity *> &AwScene::getEntityChildren(AwEntity *entity)
{
    return entity->getComponent<HierarchyComponent>().children;
}
AwEntity *AwScene::getEntityParent(AwEntity *entity)
{
    return entity->getComponent<HierarchyComponent>().parent;
}

void AwScene::update(float deltaTime)
{
    for (auto &system : m_systems)
    {
        system.second->onUpdate(deltaTime);
    }

    for (auto &system : m_systems)
    {
        system.second->afterAllSystemsUpdated(deltaTime);
    }
}

Application *AwScene::getApplication() { return m_appContext; }
} // namespace Airwave
