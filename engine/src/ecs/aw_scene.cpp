#include "ecs/aw_scene.hpp"
#include "ecs/aw_system.hpp"
#include "ecs/aw_entity.hpp"

#include "ecs/components/hierarchy_component.hpp"
#include "ecs/components/tag_component.hpp"
#include "ecs/components/transform_component.hpp"

#include "core/application.hpp"

#include <entt/entt.hpp>
namespace Airwave
{
AwScene::AwScene(Application *appContext) : m_appContext(appContext), m_adminEntity(entt::null)
{
    m_adminEntity = createEntity("AdminEntity");
    m_registry.emplace_or_replace<TagComponent>(m_adminEntity, "AdminEntity", "AdminEntity");
    m_registry.emplace<HierarchyComponent>(m_adminEntity);
}

AwScene::~AwScene() {}

entt::entity AwScene::createEntity(const std::string &name, const std::string &tag)
{
    auto entity = m_registry.create();
    m_registry.emplace<TagComponent>(entity, name, tag);
    return entity;
}
entt::entity AwScene::createDefaultEntity(const std::string &name, const std::string &tag)
{
    int a = 0;
    auto entity = createEntity(name, tag);
    m_registry.emplace<TransformComponent>(entity);
    m_registry.emplace<HierarchyComponent>(entity);
    auto &hierachy = m_registry.get<HierarchyComponent>(entity);
    setEntityParent(entity, m_adminEntity);
    return entity;
}

entt::entity AwScene::getEntity(const std::string &name)
{
    for (auto entity : m_registry.view<TagComponent>())
    {
        if (m_registry.get<TagComponent>(entity).name == name)
        {
            return entity;
        }
    }
    return entt::null;
}

void Airwave::AwScene::traverseEntity(entt::entity entity, std::function<void(entt::entity)> callback)
{
    if (m_registry.all_of<HierarchyComponent>(entity))
    {
        auto &hierarchy = m_registry.get<HierarchyComponent>(entity);
        callback(entity);
        for (auto child : hierarchy.children)
        {
            traverseEntity(child, callback);
        }
    }
}

void AwScene::destroyEntity(entt::entity entity) { m_registry.destroy(entity); }

void AwScene::destroyAllEntities() { m_registry.clear(); }

void AwScene::setEntityParent(entt::entity child, entt::entity parent)
{
    if (!isValideEntity(child) && !isValideEntity(parent))
    {
        throw std::invalid_argument("entity or parent is invalid");
    }

    // 解除原来的父子关系
    auto &oldHierarchy = m_registry.get<HierarchyComponent>(child);
    if (isValideEntity(oldHierarchy.parent))
    {
        auto &oldParentHierarchy = m_registry.get<HierarchyComponent>(oldHierarchy.parent);
        auto &children           = oldParentHierarchy.children;
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
        oldHierarchy.parent = entt::null;
    }

    // 设置新的父子关系
    oldHierarchy.parent = parent;
    auto &newHierarchy  = m_registry.get<HierarchyComponent>(parent);
    newHierarchy.children.push_back(child);
}

void AwScene::printHierarchy(entt::entity entity, int level)
{
    for (int i = 0; i < level; i++)
    {
        std::cout << "  ";
    }
    std::cout << m_registry.get<TagComponent>(entity).name << std::endl;

    auto &children = m_registry.get<HierarchyComponent>(entity).children;
    for (auto &child : children)
    {
        printHierarchy(child, level + 1);
    }
}

const std::vector<entt::entity> &AwScene::getEntityChildren(entt::entity entity)
{
    if (m_registry.all_of<HierarchyComponent>(entity))
    {
        return m_registry.get<HierarchyComponent>(entity).children;
    }
    static const std::vector<entt::entity> empty;
    return empty;
}

entt::entity AwScene::getEntityParent(entt::entity entity)
{
    if (m_registry.all_of<HierarchyComponent>(entity))
    {
        return m_registry.get<HierarchyComponent>(entity).parent;
    }
    return entt::null;
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
