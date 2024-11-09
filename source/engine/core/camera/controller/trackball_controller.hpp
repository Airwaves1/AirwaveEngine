#pragma once

#include "camera/camera.hpp"
#include <memory>
#include "event/event_observer.hpp"

namespace Airwave
{
class TrackballController
{
  public:
    TrackballController(std::shared_ptr<Camera> camera,
                        const glm::vec3 &target = glm::vec3(0.0f, 0.0f, 0.0f));
    ~TrackballController() = default;

    void setUpBasicEvent();

    void zoom(float delta);
    void rotate(const glm::vec2 &delta);
    void pan(const glm::vec2 &delta);

    void update(float deltaTime)
    {

    }

    void setZoomSpeed(float speed) { m_zoomSpeed = speed; }

    void setRotateSpeed(float speed) { m_rotateSpeed = speed; }

    void setPanSpeed(float speed) { m_panSpeed = speed; }

    void setTarget(const glm::vec3 &target) { m_target = target; }

    void setLimitDistance(float min, float max) { m_limitDistance = glm::vec2(min, max); }

  private:
    std::shared_ptr<Camera> m_camera;
    glm::vec3 m_target;        // 目标位置
    float m_zoomSpeed;         // 缩放速度
    float m_rotateSpeed;       // 旋转速度
    float m_panSpeed;          // 平移速度
    glm::vec2 m_lastMousePos;  // 上一次鼠标位置
    glm::vec2 m_limitDistance; // 限制相机距离，min为x，max为y
    bool b_isPanning;          // 是否正在平移
    bool b_isRotating;         // 是否正在旋转

    std::shared_ptr<EventObserver> m_eventObserver;
};
} // namespace Airwave