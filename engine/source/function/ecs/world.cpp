#include "world.hpp"

#include <queue>
#include "serializer.hpp"
#include "core/uuid/uuid.hpp"
#include "base_component.hpp"
#include "function/ecs/component/transform_component.hpp"
#include "function/ecs/component/camera_component.hpp"
#include "function/ecs/component/first_person_controller.hpp"
#include "function/ecs/component/mesh_component.hpp"


namespace Airwave
{
World::World()
{
    // 创建根实体
    m_adminEntity = createEntity("adminEntity", "none", true);
    m_registry.emplace<HierarchyComponent>(m_adminEntity);
    m_registry.emplace<TransformComponent>(m_adminEntity); // 添加变换组件

    m_serializer = std::make_unique<Serializer>();
    m_serializer->registerComponent<TagComponent>();
    m_serializer->registerComponent<UUIDComponent>();
    m_serializer->registerComponent<HierarchyComponent>();
    m_serializer->registerComponent<TransformComponent>();
    m_serializer->registerComponent<CameraComponent>();
    m_serializer->registerComponent<FirstPersonControllerComponent>();
    // m_serializer->registerComponent<MeshComponent>();
}

World::~World()
{
    destroyAllEntities();       // 销毁所有实体
    m_registry.clear();         // 清空注册表
    m_adminEntity = entt::null; // 重置根实体
}
entt::entity World::createEntity(const std::string &name, const std::string &tag, bool is_active)
{
    entt::entity entity = m_registry.create();
    m_registry.emplace<UUIDComponent>(entity);
    auto &tag_component     = m_registry.emplace<TagComponent>(entity);
    tag_component.tag       = tag;
    tag_component.name      = name;
    tag_component.is_active = is_active;

    return entity;
}
entt::entity World::createDefaultEntity(const std::string &name, const std::string &tag, bool is_active)
{
    entt::entity entity     = createEntity(name, tag, is_active);
    auto &entity_hierarchy  = m_registry.emplace<HierarchyComponent>(entity);
    auto &admin_hierarchy   = m_registry.get<HierarchyComponent>(m_adminEntity);
    entity_hierarchy.parent = m_adminEntity;        // 设置父实体为根实体
    admin_hierarchy.children.push_back(entity);     // 将新实体添加到根实体的子实体列表中
    m_registry.emplace<TransformComponent>(entity); // 添加变换组件
    return entity;
}
void World::destroyEntity(entt::entity &entity, bool recursive)
{
    if (entity == entt::null) return;

    auto *hierarchy = m_registry.try_get<HierarchyComponent>(entity);

    // 如果存在子节点，先递归销毁子节点
    if (hierarchy)
    {
        if (recursive)
        {
            auto children = hierarchy->children; // 复制子实体列表
            for (auto child : children)
            {
                destroyEntity(child);
            }
        }
        else
        {
            for (auto child : hierarchy->children)
            {
                auto *child_hierarchy = m_registry.try_get<HierarchyComponent>(child);
                if (child_hierarchy)
                {
                    setParent(child, m_adminEntity); // 将子实体的父实体设置为根实体
                }
            }
        }
        hierarchy->children.clear();
    }

    if (hierarchy && hierarchy->parent != entt::null)
    {
        auto *parent_hierarchy = m_registry.try_get<HierarchyComponent>(hierarchy->parent);
        if (parent_hierarchy)
        {
            // 从父实体的子实体列表中移除当前实体
            parent_hierarchy->children.erase(std::remove(parent_hierarchy->children.begin(), parent_hierarchy->children.end(), entity),
                                             parent_hierarchy->children.end());
        }
    }
    // 销毁实体（自动移除所有组件）
    if (m_registry.valid(entity))
    {
        m_registry.destroy(entity);
        entity = entt::null; // 设置为无效实体
    }
}

void World::destroyEntityByUUID(const UUID &uuid)
{
    auto view = m_registry.view<UUIDComponent>();
    for (auto entity : view)
    {
        auto &uuid_component = view.get<UUIDComponent>(entity);
        if (uuid_component.uuid == uuid)
        {
            destroyEntity(entity);
            return;
        }
    }

    LOG_WARN("Entity with UUID {} not found", uuid.toString());
}

void World::destroyEntityByName(const std::string &name)
{
    auto view = m_registry.view<TagComponent>();
    for (auto entity : view)
    {
        auto &tag_component = view.get<TagComponent>(entity);
        if (tag_component.name == name)
        {
            destroyEntity(entity);
            return;
        }
    }

    LOG_WARN("Entity with name {} not found", name);
}

void World::destroyAllEntities()
{
    auto view = m_registry.view<UUIDComponent>();
    for (auto entity : view)
    {
        destroyEntity(entity);
    }
    m_registry.clear(); // 清空注册表

    m_adminEntity = createEntity("adminEntity", "none", true); // 重新创建根实体
    m_registry.emplace<HierarchyComponent>(m_adminEntity);     // 重新添加层级组件
}

void World::setParent(entt::entity child, entt::entity parent)
{
    if (child == parent || child == entt::null || child == m_adminEntity) return;

    if (isAncestor(parent, child)) return; // 检查是否形成祖先环

    auto &child_hierarchy = m_registry.get_or_emplace<HierarchyComponent>(child);

    if (parent == entt::null)
    {
        // 如果父实体为null，直接移除子实体的父实体
        child_hierarchy.parent = m_adminEntity;
        auto &parent_hierarchy = m_registry.get_or_emplace<HierarchyComponent>(m_adminEntity);
        parent_hierarchy.children.push_back(child);
        return;
    }

    const entt::entity old_parent = child_hierarchy.parent;

    if (old_parent == parent) return; // 如果父实体没有变化，直接返回

    if (old_parent != entt::null)
    {
        auto &old_parent_hierarchy = m_registry.get_or_emplace<HierarchyComponent>(old_parent);
        // 从旧父实体的子实体列表中移除子实体
        old_parent_hierarchy.children.erase(std::remove(old_parent_hierarchy.children.begin(), old_parent_hierarchy.children.end(), child),
                                            old_parent_hierarchy.children.end());
    }

    // 设置新父实体
    child_hierarchy.parent = parent;
    auto &parent_hierarchy = m_registry.get_or_emplace<HierarchyComponent>(parent);
    parent_hierarchy.children.push_back(child); // 将子实体添加到新父实体的子实体列表中
}
void World::addChild(entt::entity parent, entt::entity child)
{
    if (parent == child || child == entt::null || child == m_adminEntity) return;

    if (isAncestor(parent, child)) return; // 检查是否形成祖先环

    auto &child_hierarchy         = m_registry.get_or_emplace<HierarchyComponent>(child);
    const entt::entity old_parent = child_hierarchy.parent;

    if (old_parent != entt::null)
    {
        removeChild(old_parent, child);
    }

    // 设置新父实体
    child_hierarchy.parent = parent;
    if (parent != entt::null)
    {
        auto &parent_hierarchy = m_registry.get_or_emplace<HierarchyComponent>(parent);
        parent_hierarchy.children.push_back(child);
    }
}
void World::removeChild(entt::entity parent, entt::entity child)
{
    if (parent == child || child == entt::null) return;

    auto &parent_hierarchy = m_registry.get_or_emplace<HierarchyComponent>(parent);
    auto &child_hierarchy  = m_registry.get_or_emplace<HierarchyComponent>(child);

    // 从父实体的子实体列表中移除子实体
    parent_hierarchy.children.erase(std::remove(parent_hierarchy.children.begin(), parent_hierarchy.children.end(), child),
                                    parent_hierarchy.children.end());

    // 清除子实体的父实体
    child_hierarchy.parent = entt::null;

    // 将子实体的父实体设置为根实体
    child_hierarchy.parent = m_adminEntity;
    auto &root_hierarchy   = m_registry.get_or_emplace<HierarchyComponent>(m_adminEntity);
    root_hierarchy.children.push_back(child); // 将子实体添加到根实体的子实体列表中
}
void World::removeFromParent(entt::entity child)
{
    if (child == entt::null) return;

    auto &child_hierarchy = m_registry.get_or_emplace<HierarchyComponent>(child);
    entt::entity parent   = child_hierarchy.parent;

    if (parent != entt::null)
    {
        removeChild(parent, child);
    }

    // 清除子实体的父实体
    child_hierarchy.parent = entt::null;

    // 将子实体的父实体设置为根实体
    setParent(child, m_adminEntity);
}
const std::vector<entt::entity> &World::getChildren(entt::entity entity) const
{
    auto *hierarchy = m_registry.try_get<HierarchyComponent>(entity);
    if (hierarchy)
    {
        return hierarchy->children;
    }
    else
    {
        static const std::vector<entt::entity> empty_children;
        return empty_children;
    }
}
entt::entity World::getParent(entt::entity entity) const
{
    auto *hierarchy = m_registry.try_get<HierarchyComponent>(entity);
    if (hierarchy)
    {
        return hierarchy->parent;
    }
    else
    {
        return entt::null;
    }
}
void World::traverseHierarchy(entt::entity entity, std::function<void(entt::entity)> func, bool include_self)
{
    if (entity == entt::null) return;

    if (include_self)
    {
        func(entity); // 执行传入的函数
    }

    auto *hierarchy = m_registry.try_get<HierarchyComponent>(entity);
    if (hierarchy == nullptr) return; // 如果没有层级组件，直接返回

    for (auto child : hierarchy->children)
    {
        traverseHierarchy(child, func); // 递归遍历子实体
    }
}

void World::traverseHierarchyUp(entt::entity entity, std::function<void(entt::entity)> func, bool include_self)
{
    if (entity == entt::null) return;
    auto *hierarchy = m_registry.try_get<HierarchyComponent>(entity);
    if (hierarchy == nullptr) return;                   // 如果没有层级组件，直接返回
    traverseHierarchyUp(hierarchy->parent, func, true); // 递归遍历父实体
    if (include_self) func(entity);                     // 执行传入的函数
}

void World::traverseHierarchyBFS(entt::entity entity, std::function<void(entt::entity)> func, bool include_self)
{
    if (entity == entt::null) return;

    std::queue<entt::entity> queue;
    if (include_self) queue.push(entity); // 如果包含自身，先将自身加入队列

    auto *hierarchy = m_registry.try_get<HierarchyComponent>(entity);
    if (hierarchy == nullptr) return; // 如果没有层级组件，直接返回

    for (auto child : hierarchy->children)
    {
        queue.push(child); // 将子实体加入队列
    }

    while (!queue.empty())
    {
        entt::entity current = queue.front();
        queue.pop();
        func(current); // 执行传入的函数

        auto *child_hierarchy = m_registry.try_get<HierarchyComponent>(current);
        if (child_hierarchy != nullptr)
        {
            for (auto child : child_hierarchy->children)
            {
                queue.push(child); // 将子实体加入队列
            }
        }
    }
}

void World::printHierarchy(entt::entity entity, int depth, bool include_self) const
{
    if (entity == entt::null) return;

    if (include_self)
    {
        auto *tag = m_registry.try_get<TagComponent>(entity);
        if (tag)
        {
            std::string indent(depth * 2, ' ');
            LOG_INFO("{}Entity: {}, Tag: {}", indent, tag->name, tag->tag);
        }
    }

    auto *hierarchy = m_registry.try_get<HierarchyComponent>(entity);
    if (hierarchy == nullptr) return; // 如果没有层级组件，直接返回

    for (auto child : hierarchy->children)
    {
        printHierarchy(child, depth + 1); // 递归打印子实体
    }
}

entt::entity World::getEntityByUUID(const UUID &uuid) const
{
    auto view = m_registry.view<UUIDComponent>();
    for (auto entity : view)
    {
        auto &uuid_component = view.get<UUIDComponent>(entity);
        if (uuid_component.uuid == uuid)
        {
            return entity;
        }
    }
    return entt::null;
}
entt::entity World::getEntityByName(const std::string &name) const
{
    auto view = m_registry.view<TagComponent>();
    for (auto entity : view)
    {
        auto &tag_component = view.get<TagComponent>(entity);
        if (tag_component.name == name)
        {
            return entity;
        }
    }
    return entt::null;
}

void World::saveScene(const std::string &scene_name)
{
    auto json = m_serializer->serializeScene(m_registry, scene_name);
    Serializer::writeToFile(scene_name + ".json", json);
}
void World::loadScene(const std::string &scene_name)
{
    auto json = Serializer::readFromFile(scene_name + ".json");
    if (json.is_null())
    {
        LOG_ERROR("Failed to load scene: {}", scene_name);
        return;
    }

    destroyAllEntities();                                   // 销毁所有实体
    m_serializer->DeserializeScene(m_registry, this, json); // 反序列化场景
}

void World::update(float deltaTime)
{
    // 根据优先级更新系统
    auto self = std::weak_ptr<World>(shared_from_this());
    for (auto &[priority, system] : m_systemsByPriority)
    {
        if (system != nullptr)
        {
            system->onUpdate(self, deltaTime);
        }
    }
}

bool World::isAncestor(entt::entity entity, entt::entity potentialAncestor) const
{
    entt::entity current = entity;
    while (current != entt::null)
    {
        if (current == potentialAncestor) return true;
        const auto *hierarchy = m_registry.try_get<HierarchyComponent>(current);
        current               = hierarchy ? hierarchy->parent : entt::null;
    }
    return false;
}
}; // namespace Airwave