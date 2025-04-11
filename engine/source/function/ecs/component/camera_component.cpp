#include "camera_component.hpp"
#include <glm/glm.hpp>
namespace Airwave
{
glm::mat4 &CameraComponent::getProjectionMatrix() const
{
    if (is_dirty)
    {
        if (projection_type == ProjectionType::Perspective)
        {
            projectionMatrix = glm::perspective(glm::radians(fov), aspect_ratio, near_clip, far_clip);
        }
        else if (projection_type == ProjectionType::Orthographic)
        {
            projectionMatrix = glm::ortho(-ortho_size * aspect_ratio / 2.0f, ortho_size * aspect_ratio / 2.0f, -ortho_size / 2.0f, ortho_size / 2.0f,
                                          near_clip, far_clip);
        }
        is_dirty = false;
    }
    return projectionMatrix;
}

glm::mat4 &CameraComponent::getViewMatrix() const { return viewMatrix; }

void CameraComponent::setProjectionType(ProjectionType type)
{
    projection_type = type;
    is_dirty        = true;
}

void CameraComponent::setAspectRatio(float aspect_ratio)
{
    this->aspect_ratio = aspect_ratio;
    is_dirty           = true;
}

void CameraComponent::setFov(float fov)
{
    this->fov = fov;
    is_dirty  = true;
}

void CameraComponent::setNearClip(float near_clip)
{
    this->near_clip = near_clip;
    is_dirty        = true;
}

void CameraComponent::setFarClip(float far_clip)
{
    this->far_clip = far_clip;
    is_dirty       = true;
}

void CameraComponent::setOrthoSize(float ortho_size)
{
    this->ortho_size = ortho_size;
    is_dirty         = true;
}

} // namespace Airwave
