#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "utils/log.hpp"

namespace Airwave
{
enum class CameraType
{
    Orthographic,
    Perspective
};

class Camera
{
  public:
    Camera() = default;
    Camera(CameraType type, float fov, float aspectRatio, float nearPlane, float farPlane)
        : m_type(type), m_fov(fov), m_aspectRatio(aspectRatio), m_nearPlane(nearPlane),
          m_farPlane(farPlane)
    {
        b_isDirty = true;
        updateViewMatrix();
        updateProjectionMatrix();
    }

    const glm::mat4 &getViewMatrix()
    {
        if (b_isDirty)
        {
            updateViewMatrix();
        }
        return m_viewMatrix;
    }

    const glm::mat4 &getProjectionMatrix()
    {
        if (b_isDirty)
        {
            updateProjectionMatrix();
        }
        return m_projectionMatrix;
    }

    glm::mat4 getViewProjectionMatrix()
    {
        if (b_isDirty)
        {
            updateViewMatrix();
            updateProjectionMatrix();
        }
        return m_projectionMatrix * m_viewMatrix;
    }

    void updateProjectionMatrix()
    {
        if (m_type == CameraType::Perspective)
        {
            m_projectionMatrix =
                glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);

        }
        else
        {
            m_projectionMatrix = glm::ortho(m_orthoLeft, m_orthoRight, m_orthoBottom, m_orthoTop,
                                            m_nearPlane, m_farPlane);
        }
        b_isDirty = false;
    }

    void updateViewMatrix()
    {
        m_viewMatrix =
            glm::mat4_cast(glm::inverse(m_rotation)) * glm::translate(glm::mat4(1.0f), -m_position);
        b_isDirty = false;
    }

    void setPosition(const glm::vec3 &position)
    {
        if (m_position != position)
        {
            m_position = position;
            b_isDirty  = true;
        }
    }

    void setRotation(const glm::quat &rotation)
    {
        if (m_rotation != rotation)
        {
            m_rotation = rotation;
            b_isDirty  = true;
        }
    }

    void setRotation(const glm::vec3 &axis, float angle)
    {
        float angleRad     = glm::radians(angle);
        glm::quat rotation = glm::angleAxis(angleRad, glm::normalize(axis));
        setRotation(rotation);
    }

    void setRotation(const glm::vec3 &eulerAngles)
    {
        glm::quat rotation = glm::quat(eulerAngles);
        setRotation(rotation);
    }

    void setWorldUp(const glm::vec3 &worldUp)
    {
        if (m_worldUp != worldUp)
        {
            m_worldUp = glm::normalize(worldUp);
            b_isDirty = true;
        }
    }

    void setFov(float fov)
    {
        if (m_fov != fov)
        {
            m_fov     = fov;
            b_isDirty = true;
        }
    }

    void setAspectRatio(float aspectRatio)
    {
        if (m_aspectRatio != aspectRatio)
        {
            m_aspectRatio = aspectRatio;
            b_isDirty     = true;
        }
    }

    void setNearPlane(float nearPlane)
    {
        if (m_nearPlane != nearPlane)
        {
            m_nearPlane = nearPlane;
            b_isDirty   = true;
        }
    }

    void setFarPlane(float farPlane)
    {
        if (m_farPlane != farPlane)
        {
            m_farPlane = farPlane;
            b_isDirty  = true;
        }
    }

    void setOrthoLeft(float orthoLeft)
    {
        if (m_orthoLeft != orthoLeft)
        {
            m_orthoLeft = orthoLeft;
            b_isDirty   = true;
        }
    }

    void setOrthoRight(float orthoRight)
    {
        if (m_orthoRight != orthoRight)
        {
            m_orthoRight = orthoRight;
            b_isDirty    = true;
        }
    }

    void setOrthoBottom(float orthoBottom)
    {
        if (m_orthoBottom != orthoBottom)
        {
            m_orthoBottom = orthoBottom;
            b_isDirty     = true;
        }
    }

    void setOrthoTop(float orthoTop)
    {
        if (m_orthoTop != orthoTop)
        {
            m_orthoTop = orthoTop;
            b_isDirty  = true;
        }
    }

    void setOrthographic(float left, float right, float bottom, float top, float nearPlane,
                         float farPlane)
    {
        m_type        = CameraType::Orthographic;
        m_orthoLeft   = left;
        m_orthoRight  = right;
        m_orthoBottom = bottom;
        m_orthoTop    = top;
        m_nearPlane   = nearPlane;
        m_farPlane    = farPlane;
        b_isDirty     = true;
    }

    void setPerspective(float fov, float aspectRatio, float nearPlane, float farPlane)
    {
        m_type        = CameraType::Perspective;
        m_fov         = fov;
        m_aspectRatio = aspectRatio;
        m_nearPlane   = nearPlane;
        m_farPlane    = farPlane;
        b_isDirty     = true;
    }

    const glm::vec3 &getPosition() const { return m_position; }
    const glm::quat &getRotation() const { return m_rotation; }
    const glm::vec3 &getWorldUp() const { return m_worldUp; }
    glm::vec3 getForwardDirection() const
    {
        return glm::normalize(m_rotation * glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 getRightDirection() const
    {
        return glm::normalize(m_rotation * glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 getUpDirection() const
    {
        return glm::normalize(m_rotation * glm::vec3(0.0f, 1.0f, 0.0f));
    }

    float getFov() const { return m_fov; }
    float getAspectRatio() const { return m_aspectRatio; }
    float getNearPlane() const { return m_nearPlane; }
    float getFarPlane() const { return m_farPlane; }

    float getOrthoLeft() const { return m_orthoLeft; }
    float getOrthoRight() const { return m_orthoRight; }
    float getOrthoBottom() const { return m_orthoBottom; }
    float getOrthoTop() const { return m_orthoTop; }

    CameraType getType() const { return m_type; }

  private:
    CameraType m_type = CameraType::Perspective;

    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 m_worldUp  = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::mat4 m_viewMatrix       = glm::mat4(1.0f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);

    bool b_isDirty = true;

    // Perspective
    float m_fov         = 45.0f;
    float m_aspectRatio = 1.0f;
    float m_nearPlane   = 0.1f;
    float m_farPlane    = 100.0f;

    // Orthographic
    float m_orthoLeft   = 0.0f;
    float m_orthoRight  = 0.0f;
    float m_orthoBottom = 0.0f;
    float m_orthoTop    = 0.0f;
};
} // namespace Airwave