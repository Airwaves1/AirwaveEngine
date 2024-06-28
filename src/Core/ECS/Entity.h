#ifndef ENTITY_H
#define ENTITY_H

#include "Node.h"
#include "Scene.h"

namespace Airwave
{

    class Entity : public Node
    {
    public:
        Entity(const entt::entity &ecsEntity, Scene *Scene)
            : m_ecsEntity(const_cast<entt::entity &>(ecsEntity)), m_Scene(Scene)
        {
        }
        virtual ~Entity() override = default;

        template <typename T, typename... Args>
        T &AddComponent(Args &&...args)
        {
            T &component = m_Scene->m_ecsRegistry.emplace<T>(m_ecsEntity, std::forward<Args>(args)...);
            return component;
        }

        template <typename T>
        bool HasComponent()
        {
            return m_Scene->m_ecsRegistry.any_of<T>(m_ecsEntity);
        }

        template <typename... T>
        bool HasAnyComponent()
        {
            return m_Scene->m_ecsRegistry.any_of<T...>(m_ecsEntity);
        }

        template <typename... T>
        bool HasAllComponent()
        {
            return m_Scene->m_ecsRegistry.all_of<T...>(m_ecsEntity);
        }

        template <typename T>
        T &GetComponent()
        {
            assert(HasComponent<T>() && "Entity does not have component!");
            return m_Scene->m_ecsRegistry.get<T>(m_ecsEntity);
        }

        template <typename T>
        void RemoveComponent()
        {
            assert(HasComponent<T>() && "Entity does not have component!");
            m_Scene->m_ecsRegistry.remove<T>(m_ecsEntity);
        }

    private:
        entt::entity &m_ecsEntity;
        Scene *m_Scene;
    };
}

#endif // ENTITY_H