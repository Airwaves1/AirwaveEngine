#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "function/global/global_context.hpp"
#include "function/ecs/base_component.hpp"
#include <nlohmann/json.hpp>
#include <queue>
#include <entt/entt.hpp>

namespace Airwave
{
struct TransformComponent
{
  public:
    constexpr static const char *getComponentName() { return "TransformComponent"; }

    enum class Space
    {
        Local,
        World,
    };

    void translate(const glm::vec3 &translation, Space space = Space::Local);
    void rotate(const glm::quat &deltaRotation, Space space = Space::Local);
    void lookAt(const glm::vec3 &target, const glm::vec3 &up = {0.0f, 1.0f, 0.0f});

    void setPosition(const glm::vec3 &position);
    void setRotation(const glm::quat &rotation);
    void setScale(const glm::vec3 &scale);

    glm::vec3 getForward() const;
    glm::vec3 getRight() const;
    glm::vec3 getUp() const;

    glm::vec3 getPosition(Space space = Space::Local) const;
    glm::quat getRotation(Space space = Space::Local) const;
    glm::vec3 getScale() const;

    glm::mat4 calculateLocalMatrix() const;
    const glm::mat4 &getWorldMatrix() const;
    const glm::mat4 &getLocalMatrix() const;
    glm::mat4 getWorldMatrixInverse() const;

    // 序列化
    nlohmann::json to_json() const;

    void from_json(const nlohmann::json &j);

    static void printMatrix(bool column_major, const glm::mat4 &matrix, const std::string &name = "Matrix")
    {
        if (column_major)
        {
            LOG_INFO("{}: \n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]", name, matrix[0][0], matrix[0][1], matrix[0][2],
                     matrix[0][3], matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3], matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
                     matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
        }
        else
        {
            LOG_INFO("{}: \n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]\n[{}, {}, {}, {}]", name, matrix[0][0], matrix[1][0], matrix[2][0],
                     matrix[3][0], matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1], matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2],
                     matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]);
        }
    }

  private:
    glm::vec3 position{0.0f, 0.0f, 0.0f};
    glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f}; // w, x, y, z
    glm::vec3 scale{1.0f, 1.0f, 1.0f};

    glm::vec3 default_forward{0.0f, 0.0f, -1.0f};
    glm::vec3 default_up{0.0f, 1.0f, 0.0f};
    glm::vec3 default_right{1.0f, 0.0f, 0.0f};

    mutable bool is_dirty{true};
    mutable glm::mat4 worldMatrix{1.0f};
    mutable glm::mat4 localMatrix{1.0f};
    mutable glm::mat4 worldMatrixInverse{1.0f};

    friend class TransformSystem;

    static std::queue<entt::entity> updateQueue;
    static void updateWorldMatrix();
};

} // namespace Airwave
