#pragma once

#include <cstdint>
#include <glm/glm.hpp>

// 轴对齐包围盒
namespace Airwave
{
namespace Math
{
class AABB
{
  public:
    glm::vec3 min;
    glm::vec3 max;

    // 初始化AABB，min和max分别是AABB的最小和最大点，设为无穷
    AABB(const glm::vec3 &min = glm::vec3(std::numeric_limits<float>::infinity()),
         const glm::vec3 &max = glm::vec3(-std::numeric_limits<float>::infinity()))
        : min(min), max(max)
    {
    }

    bool is_valid() const { return (min.x > max.x) || (min.y > max.y) || (min.z > max.z); }

    glm::vec3 get_center() const
    {
        glm::vec3 res = glm::vec3(0.0f);
        if (is_valid())
        {
            res = (min + max) * 0.5f;
        }
        return res;
    }

    glm::vec3 get_size() const
    {
        if (is_valid())
            return max - min;
        else
            return glm::vec3(0.0f);
    }

    // 判断点是否在AABB内
    bool contains_point(const glm::vec3 &point) const
    {
        return !(point.x < min.x || point.x > max.x || point.y < min.y || point.y > max.y || point.z < min.z || point.z > max.z);
    }

    // 判断是否包含另一个AABB
    bool contains_aabb(const AABB &aabb) const { return contains_point(aabb.min) && contains_point(aabb.max); }

    void extend_by_point(const glm::vec3 &point)
    {
        min = fast_min(min, point);
        max = fast_max(max, point);
    }

    // 应用变换矩阵
    AABB get_transformed(const glm::mat4 &transform) const
    {
        glm::vec3 corners[8] = {
            glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, min.z), glm::vec3(min.x, max.y, max.z),
            glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z),
        };

        AABB result;
        for (int i = 0; i < 8; i++)
        {
            glm::vec4 corner = transform * glm::vec4(corners[i], 1.0f);
            result.extend_by_point(glm::vec3(corner));
        }

        return result;
    }

  private:
    // 快速最小值计算（避免多次调用glm::min）
    static glm::vec3 fast_min(const glm::vec3 &a, const glm::vec3 &b)
    {
        return glm::vec3((a.x < b.x) ? a.x : b.x, (a.y < b.y) ? a.y : b.y, (a.z < b.z) ? a.z : b.z);
    }

    // 快速最大值计算（避免多次调用glm::max）
    static glm::vec3 fast_max(const glm::vec3 &a, const glm::vec3 &b)
    {
        return glm::vec3((a.x > b.x) ? a.x : b.x, (a.y > b.y) ? a.y : b.y, (a.z > b.z) ? a.z : b.z);
    }
};
} // namespace Math
} // namespace Airwave
