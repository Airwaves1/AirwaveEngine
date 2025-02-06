#pragma once

#include <string>
#include <optional>

#include <entt/entt.hpp>

#include "ecs/aw_scene.hpp"
#include "ecs/components/tag_component.hpp"

namespace Airwave
{
class AwEntity
{
  public:
    AwEntity(AwScene *scene, entt::entity &entity, const std::string &name,
             const std::string &tag = "")
        : m_scene(scene), m_entity(entity)
    {
    }

    AwScene *getScene() const { return m_scene; }

    bool operator==(const AwEntity &other) const { return m_entity == other.m_entity; }
    bool operator!=(const AwEntity &other) const { return m_entity != other.m_entity; }

    template <typename T> bool hasComponent() { return m_scene->getRegistry().all_of<T>(m_entity); }

    template <typename T, typename... Args> T &addComponent(Args &&...args)
    {
        if (hasComponent<T>())
        {
            return m_scene->getRegistry().get<T>(m_entity);
        }
        return m_scene->getRegistry().emplace<T>(m_entity, std::forward<Args>(args)...);
    }

    template <typename T> T &getComponent() { return m_scene->getRegistry().get<T>(m_entity); }

    template <typename T> T *tryGetComponent()
    {
        return m_scene->getRegistry().try_get<T>(m_entity);
    }

    template <typename T> void removeComponent()
    {
        if (hasComponent<T>())
        {
            m_scene->getRegistry().remove<T>(*this);
        }
    }

  private:
    AwScene *m_scene{nullptr};

    entt::entity m_entity;

    friend class AwScene;
};

} // namespace Airwave
