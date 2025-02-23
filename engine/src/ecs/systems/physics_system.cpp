#include "ecs/systems/physics_system.hpp"
#include "ecs/aw_scene.hpp"
#include "ecs/components/singleton_components/physics_world_component.hpp"
#include "ecs/components/transform_component.hpp"
#include "ecs/components/rigid_body_component.hpp"
#include "ecs/components/tag_component.hpp"

#include "ecs/events/collosion_event.hpp"
#include "ecs/events/trigger_event.hpp"

#include <entt/entt.hpp>
#include <btBulletDynamicsCommon.h>

namespace Airwave
{
void PhysicsSystem::createRigidBody(AwScene *scene, entt::entity entity)
{
    if (!scene->hasComponent<TransformComponent>(entity) || !scene->hasComponent<RigidBodyComponent>(entity))
    {
        LOG_ERROR("entity has no TransformComponent or RigidBodyComponent");
        return;
    }
    auto admin_entity     = scene->getAdminEntity();
    auto &transform_comp  = scene->getComponent<TransformComponent>(entity);
    auto &rigid_body_comp = scene->getComponent<RigidBodyComponent>(entity);

    // 创建碰撞形状
    btCollisionShape *collision_shape = nullptr;
    switch (rigid_body_comp.colliderType)
    {
        case ColliderType::Sphere:
            collision_shape = new btSphereShape(rigid_body_comp.shapeSize.x);
            break;
        case ColliderType::Box:
            collision_shape = new btBoxShape(btVector3(rigid_body_comp.shapeSize.x * 0.5,
                                                       rigid_body_comp.shapeSize.y * 0.5,
                                                       rigid_body_comp.shapeSize.z * 0.5));
            break;
        case ColliderType::Capsule:
            collision_shape = new btCapsuleShape(rigid_body_comp.shapeSize.x,
                                                 rigid_body_comp.shapeSize.y);
            break;
        case ColliderType::Mesh:
            LOG_WARN("Mesh collider is not supported yet");
            break;
        default:
            break;
    }

    // 设置运动状态
    btTransform bt_transform;
    bt_transform.setIdentity();
    bt_transform.setOrigin(btVector3(transform_comp.m_position.x, transform_comp.m_position.y, transform_comp.m_position.z));
    bt_transform.setRotation(
        btQuaternion(transform_comp.m_rotation.x, transform_comp.m_rotation.y, transform_comp.m_rotation.z, transform_comp.m_rotation.w));

    // 计算惯性
    btVector3 inertia(0, 0, 0);
    if (rigid_body_comp.mass > 0.0)
    {
        collision_shape->calculateLocalInertia(rigid_body_comp.mass, inertia);
    }

    // 创建刚体
    btMotionState *motion_state = new btDefaultMotionState(bt_transform);
    btRigidBody::btRigidBodyConstructionInfo rigid_body_info(rigid_body_comp.mass, motion_state, collision_shape, inertia);
    rigid_body_comp.rigidBody = std::make_unique<btRigidBody>(rigid_body_info);

    if (rigid_body_comp.rigidBody == nullptr)
    {
        LOG_ERROR("rigid_body_comp.rigidBody is nullptr");
        return;
    }

    // 设置是否为触发器
    if (rigid_body_comp.isTrigger)
    {
        rigid_body_comp.rigidBody->setCollisionFlags(rigid_body_comp.rigidBody->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
    }

    // 添加到物理世界
    auto physics_world_comp = scene->getComponent<PhysicsWorldComponent>(admin_entity);
    physics_world_comp.dynamicsWorld->addRigidBody(rigid_body_comp.rigidBody.get());

    // 设置实体索引
    rigid_body_comp.rigidBody->setUserIndex(static_cast<int>(entity));
}

void PhysicsSystem::onUpdate(float deltaTime)
{
    auto admin_entity        = m_scene->getAdminEntity();
    auto &physics_world_comp = m_scene->getComponent<PhysicsWorldComponent>(admin_entity);

    // 同步ecs变换到bullet
    auto view = m_scene->getRegistry().view<TransformComponent, RigidBodyComponent>();
    for (auto entity : view)
    {
        auto &transform_comp  = view.get<TransformComponent>(entity);
        auto &rigid_body_comp = view.get<RigidBodyComponent>(entity);

        //
        if (rigid_body_comp.rigidBody == nullptr)
        {
            createRigidBody(m_scene, entity);
        }

        if (rigid_body_comp.mass == 0.0f) // 静态物体更新物理状态
        {
            btTransform bt_transform;
            bt_transform.setIdentity();
            bt_transform.setOrigin(btVector3(transform_comp.m_position.x, transform_comp.m_position.y, transform_comp.m_position.z));
            bt_transform.setRotation(
                btQuaternion(transform_comp.m_rotation.x, transform_comp.m_rotation.y, transform_comp.m_rotation.z, transform_comp.m_rotation.w));
            rigid_body_comp.rigidBody->setWorldTransform(bt_transform);

            rigid_body_comp.rigidBody->setWorldTransform(bt_transform);
            rigid_body_comp.rigidBody->activate(); // 激活
        }

        if (!rigid_body_comp.isActivate)
        {
            btTransform bt_transform;
            bt_transform.setIdentity();
            bt_transform.setOrigin(btVector3(transform_comp.m_position.x, transform_comp.m_position.y, transform_comp.m_position.z));
            bt_transform.setRotation(
                btQuaternion(transform_comp.m_rotation.x, transform_comp.m_rotation.y, transform_comp.m_rotation.z, transform_comp.m_rotation.w));
            rigid_body_comp.rigidBody->setWorldTransform(bt_transform);

            rigid_body_comp.rigidBody->setWorldTransform(bt_transform);
            rigid_body_comp.rigidBody->activate(); // 激活
        }
    }

    // 物理模拟
    physics_world_comp.dynamicsWorld->stepSimulation(deltaTime, 10);

    // 同步bullet变换到ecs
    for (auto entity : view)
    {
        auto &transform_comp  = view.get<TransformComponent>(entity);
        auto &rigid_body_comp = view.get<RigidBodyComponent>(entity);

        if (rigid_body_comp.mass > 0.0f) // 动态物体更新ecs变换
        {
            btTransform bt_transform;
            rigid_body_comp.rigidBody->getMotionState()->getWorldTransform(bt_transform);

            glm::vec3 pos = glm::vec3(bt_transform.getOrigin().getX(), bt_transform.getOrigin().getY(), bt_transform.getOrigin().getZ());
            glm::quat rot = glm::quat(bt_transform.getRotation().getW(), bt_transform.getRotation().getX(), bt_transform.getRotation().getY(),
                                      bt_transform.getRotation().getZ());

            transform_comp.setPosition(pos);
            transform_comp.setRotation(rot);
        }

        // 处理碰撞事件
        processCollisionEvent();
    }
}

void PhysicsSystem::processCollisionEvent()
{
    auto admin_entity        = m_scene->getAdminEntity();
    auto &registry           = m_scene->getRegistry();
    auto &dispatcher         = m_scene->getDispatcher();
    auto &physics_world_comp = m_scene->getComponent<PhysicsWorldComponent>(admin_entity);

    // 碰撞对的数量
    int numManifolds = physics_world_comp.dynamicsWorld->getDispatcher()->getNumManifolds();

    // 用于存储碰撞对，避免重复处理
    std::unordered_set<entt::entity> processedPairs;

    for (int i = 0; i < numManifolds; ++i)
    {
        btPersistentManifold *contactManifold = physics_world_comp.dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

        // 获取碰撞的两个物体
        const btCollisionObject *objectA = contactManifold->getBody0();
        const btCollisionObject *objectB = contactManifold->getBody1();

        // 获取碰撞的两个实体
        entt::entity entityA = static_cast<entt::entity>(objectA->getUserIndex());
        entt::entity entityB = static_cast<entt::entity>(objectB->getUserIndex());

        // 跳过无效实体或已经销毁的实体
        if (!m_scene->isValideEntity(entityA) || !m_scene->isValideEntity(entityB))
        {
            continue;
        }

        // 生成唯一碰撞对的key
        auto pairKey = std::minmax(entityA, entityB);
        auto pair    = std::make_pair(pairKey.first, pairKey.second);

        // 检查是否已经处理过
        if (processedPairs.find(pairKey.first) != processedPairs.end() && processedPairs.find(pairKey.second) != processedPairs.end())
        {
            continue;
        }
        processedPairs.insert(pairKey.first);
        processedPairs.insert(pairKey.second);

        // 获取刚体组件判断是否为触发器
        auto &rigidBodyA = m_scene->getComponent<RigidBodyComponent>(entityA);
        auto &rigidBodyB = m_scene->getComponent<RigidBodyComponent>(entityB);

        // 处理触点数据
        for (int j = 0; j < contactManifold->getNumContacts(); ++j)
        {
            btManifoldPoint &pt = contactManifold->getContactPoint(j);

            if (pt.getDistance() < 0.0f) // 有效碰撞
            {
                // 转换到引擎空间
                glm::vec3 contactPos = glm::vec3(pt.getPositionWorldOnA().getX(), pt.getPositionWorldOnA().getY(), pt.getPositionWorldOnA().getZ());
                glm::vec3 contactNormal = glm::vec3(pt.m_normalWorldOnB.getX(), pt.m_normalWorldOnB.getY(), pt.m_normalWorldOnB.getZ());

                // 触发器特殊处理
                if (rigidBodyA.isTrigger || rigidBodyB.isTrigger)
                {
                    dispatcher.enqueue<TriggerEvent>({entityA, entityB, contactPos, contactNormal});
                }
                else
                {
                    dispatcher.enqueue<CollisionEvent>({entityA, entityB, contactPos, contactNormal, pt.getAppliedImpulse()});
                }
            }
        }

        // 清除碰撞数据
        contactManifold->clearManifold();
    }

    // 处理触发器事件
    dispatcher.update<TriggerEvent>();
    dispatcher.update<CollisionEvent>();
}

} // namespace Airwave
