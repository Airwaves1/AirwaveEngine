#pragma once

#include <entt/entity/registry.hpp>
#include <memory>
#include <optional>
#include "scene.hpp"
#include "uuid.hpp"
#include "utils/log.hpp"

namespace Airwave
{
struct AwEntitySpecification
{
    std::string name;

    UUID uuid;

    AwEntitySpecification() : uuid(UUID::Generate()) {}
    AwEntitySpecification(const std::string &name) : name(name), uuid(UUID::Generate()) {}
};

class AwEntity : public std::enable_shared_from_this<AwEntity>
{
  public:
    AwEntity(const std::shared_ptr<Scene> &scene, const entt::entity &entity,
             const AwEntitySpecification &specification)
        : m_scene(scene), m_entity(entity), m_specification(specification)
    {
    }

    ~AwEntity()
    {
        for (auto &child : m_children)
        {
            child->m_parent.reset(); // 使用reset而非setParent(nullptr)，避免不必要的递归调用
        }
    }

    // 添加组件
    template <typename T, typename... Args> T &addComponent(Args &&...args)
    {
        return m_scene->getRegistry()->emplace<T>(m_entity, std::forward<Args>(args)...);
    }

    // 添加组件
    template <typename T> T &addComponent(const T &component)
    {
        return m_scene->getRegistry()->emplace_or_replace<T>(m_entity, component);
    }

    // 获取组件
    template <typename T> T &getComponent()
    {
        if (hasComponent<T>())
            return m_scene->getRegistry()->get<T>(m_entity);
        else
        {
            LOG_ERROR("Entity does not have component: {0}", typeid(T).name());
            throw std::runtime_error("Component not found");
        }
    }

    // 移除组件
    template <typename T> void removeComponent()
    {
        if (hasComponent<T>()) m_scene->getRegistry()->remove<T>(m_entity);
    }

    // 判断是否包含某个组件
    template <typename T> bool hasComponent() const
    {
        if (m_scene == nullptr) return false;
        auto registry = m_scene->getRegistry();
        bool has      = registry->all_of<T>(m_entity);
        return has;
    }

    // 判断是否包含所有组件
    template <typename... T> bool hasComponents() const
    {
        return m_scene->getRegistry()->all_of<T...>(m_entity);
    }

    // 判断是否包含任意组件
    template <typename... T> bool hasAnyComponent() const
    {
        return m_scene->getRegistry()->any_of<T...>(m_entity);
    }

    // 获取entt实体
    entt::entity getEntity() const { return m_entity; }

    // 获取实体的 UUID
    UUID getUUID() const { return m_specification.uuid; }

    // 获取实体的名称
    std::string getName() const { return m_specification.name; }

    // 设置实体的名称
    void setName(const std::string &name) { m_specification.name = name; }

    // 获取父实体
    std::shared_ptr<AwEntity> getParent() const { return m_parent.lock(); }

    // 设置父实体
    void setParent(const std::shared_ptr<AwEntity> &parent)
    {
        if (m_parent.lock().get() == parent.get()) return;

        if (auto oldParent = m_parent.lock()) oldParent->removeChild(shared_from_this());

        m_parent = parent;

        if (parent) parent->addChild(shared_from_this());
    }

    // 判断是否有父实体
    bool hasParent() const { return !m_parent.expired(); }

    // 获取子实体列表
    const std::vector<std::shared_ptr<AwEntity>> &getChildren() const { return m_children; }

    // 添加子实体
    void addChild(const std::shared_ptr<AwEntity> &child)
    {
        if (hasChild(child) || child->getParent() == shared_from_this()) return;

        if (auto oldParent = child->getParent()) oldParent->removeChild(child);

        m_children.push_back(child);
        child->m_parent = shared_from_this(); // 避免递归调用
    }

    // 移除子实体
    void removeChild(const std::shared_ptr<AwEntity> &child)
    {
        auto it = std::find(m_children.begin(), m_children.end(), child);
        if (it == m_children.end()) return;

        m_children.erase(it);
        child->m_parent.reset(); // 直接reset而非setParent(nullptr)，避免递归
    }

    // 判断是否有某个子实体
    bool hasChild(const std::shared_ptr<AwEntity> &child) const
    {
        return std::find(m_children.begin(), m_children.end(), child) != m_children.end();
    }

    std::shared_ptr<Scene> getScene() const { return m_scene; }

  private:
    AwEntitySpecification m_specification;

    entt::entity m_entity;

    std::shared_ptr<Scene> m_scene;

    std::vector<std::shared_ptr<AwEntity>> m_children;

    std::weak_ptr<AwEntity> m_parent;
};
} // namespace Airwave
