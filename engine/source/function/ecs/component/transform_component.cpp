#include "transform_component.hpp"
#include "core/log/log.hpp"

namespace Airwave
{

void TransformComponent::translate(const glm::vec3 &translation, Space space)
{
    if (space == Space::Local)
    {
        // 本地空间平移需要考虑当前物体的旋转
        position += rotation * translation;
    }
    else
    {
        // 世界空间平移直接加上平移向量
        position += translation;
    }

    is_dirty = true;
}

void TransformComponent::rotate(const glm::quat &deltaRotation, Space space)
{
    if (space == Space::Local)
    {
        rotation = rotation * deltaRotation; // 本地空间旋转
    }
    else
    {
        rotation = deltaRotation * rotation; // 世界空间旋转
    }

    rotation = glm::normalize(rotation); // 归一化四元数

    is_dirty = true;
}

void TransformComponent::lookAt(const glm::vec3 &target, const glm::vec3 &up)
{
    const glm::mat4 lookMat = glm::lookAt(position, target, up);
    rotation                = glm::quat_cast(glm::inverse(lookMat));

    is_dirty = true;
}

void TransformComponent::setPosition(const glm::vec3 &position)
{
    this->position = position;
    is_dirty       = true;
}

void TransformComponent::setRotation(const glm::quat &rotation)
{
    this->rotation = rotation;
    this->rotation = glm::normalize(this->rotation); // 归一化四元数
    is_dirty       = true;
}

void TransformComponent::setScale(const glm::vec3 &scale)
{
    this->scale = scale;
    is_dirty    = true;
}

glm::vec3 TransformComponent::getForward() const { return rotation * default_forward; }

glm::vec3 TransformComponent::getRight() const { return rotation * default_right; }

glm::vec3 TransformComponent::getUp() const { return rotation * default_up; }

glm::vec3 TransformComponent::getPosition(Space space) const
{
    if (space == Space::Local)
    {
        return position; // 返回本地空间位置
    }
    else
    {
        // 返回世界空间位置
        return glm::vec3(worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2]); // 从世界矩阵中获取位置
    }
}

glm::quat TransformComponent::getRotation(Space space) const
{
    if (space == Space::Local)
    {
        return rotation; // 返回本地空间旋转
    }
    else
    {
        // 返回世界空间旋转
        return glm::normalize(glm::quat_cast(worldMatrix)); // 从世界矩阵中获取旋转
    }
}

glm::vec3 TransformComponent::getScale() const
{
    return scale; // 返回缩放比例
}

glm::mat4 TransformComponent::calculateLocalMatrix() const
{
    // 计算本地矩阵
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position); // 平移矩阵
    glm::mat4 rotationMatrix    = glm::toMat4(rotation);                     // 旋转矩阵
    glm::mat4 scaleMatrix       = glm::scale(glm::mat4(1.0f), scale);        // 缩放矩阵

    localMatrix = translationMatrix * rotationMatrix * scaleMatrix; // 本地矩阵 = 平移矩阵 * 旋转矩阵 * 缩放矩阵

    return localMatrix; // 返回本地矩阵
}

const glm::mat4 &TransformComponent::getWorldMatrix() const
{
    if (is_dirty)
    {
        updateWorldMatrix();
    }
    return worldMatrix;
}

const glm::mat4 &TransformComponent::getLocalMatrix() const
{
    if (is_dirty)
    {
        updateWorldMatrix();
    }
    return localMatrix;
}

glm::mat4 TransformComponent::getWorldMatrixInverse() const
{
    if (is_dirty)
    {
        updateWorldMatrix();
    }
    return worldMatrixInverse; // 返回世界矩阵的逆矩阵
}

nlohmann::json TransformComponent::to_json() const
{
    return {{"position", {position.x, position.y, position.z}},
            {"rotation", {rotation.w, rotation.x, rotation.y, rotation.z}},
            {"scale", {scale.x, scale.y, scale.z}}};
}

void TransformComponent::from_json(const nlohmann::json &j)

{
    if (j.contains("position"))
    {
        position = glm::vec3(j["position"][0], j["position"][1], j["position"][2]);
    }
    if (j.contains("rotation"))
    {
        rotation = glm::quat(j["rotation"][0], j["rotation"][1], j["rotation"][2], j["rotation"][3]);
        rotation = glm::normalize(rotation); // 归一化四元数
    }
    if (j.contains("scale"))
    {
        scale = glm::vec3(j["scale"][0], j["scale"][1], j["scale"][2]);
    }

    is_dirty = true; // 标记为脏数据，需要更新世界矩阵
}

std::queue<entt::entity> TransformComponent::updateQueue = std::queue<entt::entity>();
void TransformComponent::updateWorldMatrix()
{
    auto world           = GlobalContext.world;
    auto &registry = world->getRegistry();
    auto adminEntity     = world->getAdminEntity();

    // 初始从根节点开始，加入队列
    updateQueue = std::queue<entt::entity>();
    updateQueue.push(adminEntity);

    // 广度优先遍历所有层级的节点
    while (!updateQueue.empty())
    {
        // 取出队列中的节点
        entt::entity entity = updateQueue.front();
        updateQueue.pop();

        // 只处理包含 TransformComponent 的实体
        if (registry.all_of<TransformComponent>(entity))
        {
            auto &transform = registry.get<TransformComponent>(entity);

            // 获取当前节点的所有子节点，并将它们加入队列，确保它们在父节点之后被更新
            auto children = world->getChildren(entity);
            for (auto child : children)
            {
                if (transform.is_dirty)
                {
                    // 如果当前节点是脏的，子节点也需要更新
                    auto &childTransform    = registry.get<TransformComponent>(child);
                    childTransform.is_dirty = true;
                }
                updateQueue.push(child);
            }

            // 只有标记为脏的节点才需要更新
            if (transform.is_dirty)
            {
                // 更新本地矩阵
                transform.worldMatrix = transform.calculateLocalMatrix();
                transform.is_dirty    = false; // 清除脏标志

                // 获取父节点
                auto parent = world->getParent(entity);
                if (parent != entt::null && registry.all_of<TransformComponent>(parent))
                {
                    auto &parentTransform = registry.get<TransformComponent>(parent);
                    // 更新当前节点的世界矩阵
                    transform.worldMatrix = parentTransform.worldMatrix * transform.worldMatrix;
                }

                transform.worldMatrixInverse = glm::inverse(transform.worldMatrix); // 更新世界矩阵的逆矩阵
            }
        }
    }
}

} // namespace Airwave
