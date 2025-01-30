#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ecs/aw_component.hpp"
namespace Airwave
{

class CameraComponent : public AwComponent
{
  public:
    enum class CameraType
    {
        Perspective, // 透视相机
        Orthographic // 正交相机
    };

    bool enable = true;

    CameraComponent()
        : m_cameraType(CameraType::Perspective), m_fov(45.0f), m_aspectRatio(16.0f / 9.0f),
          m_nearPlane(0.1f), m_farPlane(100.0f), m_orthoSize(10.0f), m_dirty(true)
    {
    }

    // 设置相机类型
    void setCameraType(CameraType type)
    {
        if (m_cameraType != type)
        {
            m_cameraType = type;
            m_dirty      = true;
        }
    }

    // 设置透视相机参数
    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        m_cameraType  = CameraType::Perspective;
        m_fov         = fov;
        m_aspectRatio = aspectRatio;
        m_nearPlane   = nearPlane;
        m_farPlane    = farPlane;
        m_dirty       = true;
    }

    // 设置正交相机参数
    void setOrthographic(float orthoSize, float aspectRatio, float nearPlane, float farPlane)
    {
        m_cameraType  = CameraType::Orthographic;
        m_orthoSize   = orthoSize;
        m_aspectRatio = aspectRatio;
        m_nearPlane   = nearPlane;
        m_farPlane    = farPlane;
        m_dirty       = true;
    }

    // 更新相机矩阵
    void update()
    {
        if (!m_dirty) return;

        if (m_cameraType == CameraType::Perspective)
        {
            m_projectionMatrix =
                glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
        }
        else if (m_cameraType == CameraType::Orthographic)
        {
            float halfWidth  = m_orthoSize * m_aspectRatio * 0.5f;
            float halfHeight = m_orthoSize * 0.5f;
            m_projectionMatrix =
                glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_nearPlane, m_farPlane);
        }

        m_dirty = false;
    }

    const glm::mat4 getProjectionMatrix() { return m_projectionMatrix; }

    const glm::mat4 getWorldInverseMatrix() { return m_worldInverseMatrix; }

    // 设置、获取属性
    CameraType getCameraType() const { return m_cameraType; }
    float getFOV() const { return m_fov; }
    float getAspectRatio() const { return m_aspectRatio; }
    float getNearPlane() const { return m_nearPlane; }
    float getFarPlane() const { return m_farPlane; }
    float getOrthoSize() const { return m_orthoSize; }

    const glm::vec3 &getCameraPosition() { return m_position; }

    void setFOV(float fov)
    {
        m_fov   = fov;
        m_dirty = true;
    }

    void setAspectRatio(float aspectRatio)
    {
        m_aspectRatio = aspectRatio;
        m_dirty       = true;
    }

    void setNearPlane(float nearPlane)
    {
        m_nearPlane = nearPlane;
        m_dirty     = true;
    }

    void setFarPlane(float farPlane)
    {
        m_farPlane = farPlane;
        m_dirty    = true;
    }

    void setOrthoSize(float orthoSize)
    {
        m_orthoSize = orthoSize;
        m_dirty    = true;
    }
    

  private:
    CameraType m_cameraType; // 相机类型

    glm::vec3 m_position = glm::vec3(0.0);

    float m_fov;         // 透视相机的视野角度
    float m_aspectRatio; // 宽高比
    float m_nearPlane;   // 近剪裁面
    float m_farPlane;    // 远剪裁面

    float m_orthoSize; // 正交相机的范围大小

    glm::mat4 m_projectionMatrix;   // 投影矩阵
    glm::mat4 m_worldInverseMatrix; // 相机世界矩阵的逆

    bool m_dirty; // 标记是否需要更新投影矩阵

    friend class CameraSystem;
};

} // namespace Airwave
