#pragma once

#include "camera/camera.hpp"
#include <memory>
#include "event/event_observer.hpp"
#include <unordered_map>

namespace Airwave
{
class FPSCameraController
{
  public:
    FPSCameraController(std::shared_ptr<Camera> camera = nullptr);
    ~FPSCameraController() {}

    void setUpBasicEvent();
    void update(float deltaTime);
    void setCamera(std::shared_ptr<Camera> camera) { m_camera = camera; }
    std::shared_ptr<Camera> getCamera() { return m_camera; }
    void setSpeed(float movementSpeed, float rotateSpeed)
    {
        m_movementSpeed = movementSpeed;
        m_rotateSpeed   = rotateSpeed;
    }

  private:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<EventObserver> m_eventObserver;

    // bool b_needHandle = false;

    // 记录按键状态
    std::unordered_map<int, bool> m_keyStates;

    glm::vec2 m_lastMousePos{0.0};
    glm::vec2 m_deltaRotation{0.0};
    bool b_isDragging = false;

    float m_yaw{0.0f};
    float m_pitch{0.0f};

    // 参数
    float m_movementSpeed = 2.0f;
    float m_rotateSpeed   = 0.002f;

    // 平移速度
    const float m_panSpeed = 0.02f;

    void handleMovement(float deltaTime);
    void handleRotation(float deltaTime);
};
} // namespace Airwave
