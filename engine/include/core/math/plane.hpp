#pragma once

#include <glm/glm.hpp>

namespace Airwave
{
namespace Math
{
class Plane
{
  public:
    glm::vec3 normal;
    float d;

    Plane(const glm::vec3 &normal = glm::vec3(0.0f), float d = 0.0f) : normal(glm::normalize(normal)), d(d) {}

    // 通过法线+平面上一点构造
    Plane(const glm::vec3 &normal, const glm::vec3 &point) 
        : normal(glm::normalize(normal)), d(-glm::dot(glm::normalize(normal), point)) {}

    // 通过3点构造平面
    Plane(const glm::vec3 &point1, const glm::vec3 &point2, const glm::vec3 &point3)
    {
        normal = glm::normalize(glm::cross(point2 - point1, point3 - point1));
        d      = -glm::dot(normal, point1);
    }

    // 计算点到平面的距离
    float distance(const glm::vec3 &point) const { return glm::dot(normal, point) + d; }

    // 计算点在平面上的投影
    glm::vec3 project(const glm::vec3 &point) const { return point - distance(point) * normal; }

    // 计算点相对于平面的反射点
    glm::vec3 reflect(const glm::vec3 &point) const { return point - 2.0f * distance(point) * normal; }
};

} // namespace Math
} // namespace Airwave
