#ifndef SCENE_H
#define SCENE_H

#include "UUID.h"
#include "entt/entity/registry.hpp"

namespace Airwave
{
    class Entity;
    class Node;

    class Scene
    {
    public:
        Scene() = default;
        virtual ~Scene() = default;

    private:
        std::string m_name;
        entt::registry m_ecsRegistry;

        std::unordered_map<entt::entity, std::shared_ptr<Entity>> m_entityMap;
        std::shared_ptr<Node> m_rootNode;

        friend class Entity;
    };

} // namespace Airwave

#endif // SCENE_H