#include "trackball_controller.hpp"
#include "utils/log.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Airwave
{
TrackballController::TrackballController(std::shared_ptr<Camera> camera, const glm::vec3 &target)
    : m_camera(camera), m_target(target), m_zoomSpeed(0.2f), m_rotateSpeed(0.01f), m_panSpeed(0.02f),
      m_lastMousePos(0.0f), m_limitDistance(0.1f, 100.0f), b_isPanning(false), b_isRotating(false)
{
    setUpBasicEvent();
}

void TrackballController::setUpBasicEvent()
{
    if (m_eventObserver == nullptr)
    {
        m_eventObserver = std::make_shared<EventObserver>();
    }

    m_eventObserver->onEvent<MouseButtonPressedEvent>(
        [this](const MouseButtonPressedEvent &e)
        {
            if (e.GetMouseButton() == MouseButton::MOUSE_BUTTON_LEFT)
            {
                b_isRotating = true;
                b_isPanning  = false;

                m_lastMousePos = glm::vec2(e.GetXPos(), e.GetYPos());
            }
            if (e.GetMouseButton() == MouseButton::MOUSE_BUTTON_MIDDLE)
            {
                b_isPanning  = true;
                b_isRotating = false;

                m_lastMousePos = glm::vec2(e.GetXPos(), e.GetYPos());
            }
        });

    m_eventObserver->onEvent<MouseButtonReleasedEvent>(
        [this](const MouseButtonReleasedEvent &e)
        {
            if (e.GetMouseButton() == MouseButton::MOUSE_BUTTON_LEFT)
            {
                b_isRotating = false;
            }
            if (e.GetMouseButton() == MouseButton::MOUSE_BUTTON_MIDDLE)
            {
                b_isPanning = false;
            }
        });

    m_eventObserver->onEvent<MouseScrolledEvent>([this](const MouseScrolledEvent &e)
                                                 { zoom(e.getOffsetY()); });

    m_eventObserver->onEvent<MouseMovedEvent>(
        [this](const MouseMovedEvent &e)
        {
            if (b_isRotating)
            {
                glm::vec2 delta = glm::vec2(e.getX(), e.getY()) - m_lastMousePos;
                m_lastMousePos  = glm::vec2(e.getX(), e.getY());
                rotate(delta);
            }
            if (b_isPanning)
            {
                glm::vec2 delta = glm::vec2(e.getX(), e.getY()) - m_lastMousePos;
                m_lastMousePos  = glm::vec2(e.getX(), e.getY());
                pan(delta);
            }
        });
}

void TrackballController::zoom(float delta)
{
    float distance = glm::distance(m_camera->getPosition(), m_target);
    distance -= delta * m_zoomSpeed;

    // 限制相机距离
    distance = glm::clamp(distance, m_limitDistance.x, m_limitDistance.y);

    // 计算新的相机位置
    glm::vec3 dir = glm::normalize(m_camera->getPosition() - m_target);
    m_camera->setPosition(m_target + dir * distance);
}

void TrackballController::rotate(const glm::vec2 &delta)
{
    glm::vec3 dir = m_camera->getPosition() - m_target;
    float distance = glm::length(dir);

    // 计算旋转角度
    glm::vec3 right = m_camera->getRightDirection();
    glm::vec3 up    = m_camera->getUpDirection();

    glm::quat rot = glm::angleAxis(-delta.y * m_rotateSpeed, right) *
                    glm::angleAxis(-delta.x * m_rotateSpeed, up);

    // 计算新的相机位置
    dir = glm::normalize(glm::rotate(rot, dir));
    m_camera->setPosition(m_target + dir * distance);

    // 计算新的相机旋转
    glm::quat newRot = glm::normalize(rot * m_camera->getRotation());
    m_camera->setRotation(newRot);
}


void TrackballController::pan(const glm::vec2 &delta)
{
    glm::vec3 dir  = m_camera->getPosition() - m_target;
    float distance = glm::length(dir);

    // 计算平移距离
    glm::vec3 right = m_camera->getRightDirection();
    glm::vec3 up    = m_camera->getUpDirection();
    glm::vec3 pan   = -right * delta.x * m_panSpeed + up * delta.y * m_panSpeed;

    // 计算新的相机位置
    m_camera->setPosition(m_camera->getPosition() + pan);
    m_target += pan;
}

} // namespace Airwave
