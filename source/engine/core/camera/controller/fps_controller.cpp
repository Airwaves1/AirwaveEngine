#include "fps_controller.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "utils/key_codes.hpp"

namespace Airwave
{
FPSCameraController::FPSCameraController(std::shared_ptr<Camera> camera) : m_camera(camera)
{
    setUpBasicEvent();
}

void FPSCameraController::setUpBasicEvent()
{
    if (m_eventObserver == nullptr) m_eventObserver = std::make_shared<EventObserver>();

    // 记录键盘按键状态
    m_eventObserver->onEvent<KeyPressedEvent>(
        [this](const KeyPressedEvent &e)
        {
            // b_needHandle               = true;
            m_keyStates[e.getKeycode()] = true;
        });

    m_eventObserver->onEvent<KeyReleasedEvent>([this](const KeyReleasedEvent &e)
                                               { m_keyStates[e.getKeycode()] = false; });

    m_eventObserver->onEvent<MouseButtonPressedEvent>(
        [this](const MouseButtonPressedEvent &e)
        {
            if (e.GetMouseButton() == AW_MOUSE_BUTTON_2)
            {
                // b_needHandle  = true;
                b_isDragging   = true;
                m_lastMousePos = glm::vec2(e.GetXPos(), e.GetYPos());
            }
        });

    m_eventObserver->onEvent<MouseButtonReleasedEvent>(
        [this](const MouseButtonReleasedEvent &e)
        {
            if (e.GetMouseButton() == AW_MOUSE_BUTTON_2)
            {
                // b_needHandle    = true;
                b_isDragging    = false;
                m_deltaRotation = glm::vec2(0.0);
            }
        });

    m_eventObserver->onEvent<MouseMovedEvent>(
        [this](const MouseMovedEvent &e)
        {
            if (!b_isDragging) return;
            // b_needHandle    = true;
            glm::vec2 mousePos(e.getX(), e.getY());
            m_deltaRotation = mousePos - m_lastMousePos;
            m_lastMousePos  = mousePos;
        });
}

void FPSCameraController::update(float deltaTime)
{
    handleMovement(deltaTime);
    handleRotation(deltaTime);
    // b_needHandle = false;
}

void FPSCameraController::handleMovement(float deltaTime)
{
    // if(!b_needHandle) return;
    if (m_camera == nullptr) return;

    glm::vec3 front = glm::normalize(m_camera->getForwardDirection());
    glm::vec3 right = glm::normalize(m_camera->getRightDirection());
    glm::vec3 up    = glm::normalize(m_camera->getUpDirection());

    glm::vec3 velocity(0.0f);

    if (m_keyStates[AW_KEY_W]) velocity += front * m_movementSpeed * deltaTime;
    if (m_keyStates[AW_KEY_S]) velocity -= front * m_movementSpeed * deltaTime;
    if (m_keyStates[AW_KEY_A]) velocity -= right * m_movementSpeed * deltaTime;
    if (m_keyStates[AW_KEY_D]) velocity += right * m_movementSpeed * deltaTime;
    if (m_keyStates[AW_KEY_Q]) velocity -= up * m_movementSpeed * deltaTime;
    if (m_keyStates[AW_KEY_E]) velocity += up * m_movementSpeed * deltaTime;

    m_camera->setPosition(m_camera->getPosition() + velocity);
}

void FPSCameraController::handleRotation(float deltaTime)
{
    if (!b_isDragging || m_camera == nullptr) return;

    float yaw   = m_deltaRotation.x * m_rotateSpeed;
    float pitch = m_deltaRotation.y * m_rotateSpeed;

    m_yaw -= yaw; // 偏航方向通常保持不变
    m_pitch = glm::clamp(m_pitch - pitch, -glm::half_pi<float>(), glm::half_pi<float>()); // 改为减去
                                                                                          // pitch

    // 先应用偏航，再应用俯仰
    glm::quat yawQuat   = glm::angleAxis(m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitchQuat = glm::angleAxis(m_pitch, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::quat rotation = yawQuat * pitchQuat; // 注意顺序
    m_camera->setRotation(glm::normalize(rotation));
}

} // namespace Airwave
