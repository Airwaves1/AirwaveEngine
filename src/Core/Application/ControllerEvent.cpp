#include "Application/ControllerEvent.h"
#include "ControllerEvent.h"

#include "Controller/CameraController.h"
#include "Windows/window.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Application/EventManager.h"

#include "Utils/Log.h"

namespace Airwave{

    ControllerEvent* ControllerEvent::m_instance = nullptr;
    CameraController* ControllerEvent::m_cameraController = nullptr;
    Window* ControllerEvent::m_window = nullptr;

    ControllerEvent* ControllerEvent::GetInstance()
    {
        if(m_instance == nullptr){
            m_instance = new ControllerEvent();
        }
        return m_instance;
    }

    void ControllerEvent::SetEvent(CameraController *cameraController, Window *window)
    {
        if(m_window == nullptr || m_cameraController == nullptr){
            m_window = window;
            m_cameraController = cameraController;
        }
    
        EventManager::GetInstance()->SetResizeCallback(m_instance->OnResize);
        EventManager::GetInstance()->SetKeyBoradCallback(m_instance->OnKey);
        EventManager::GetInstance()->SetMouseCallback(m_instance->OnMouse);
        EventManager::GetInstance()->SetCursorPosCallback(m_instance->OnCursorPos);
        EventManager::GetInstance()->SetScrollCallback(m_instance->OnScroll);
    }

    void ControllerEvent::OnResize(int width, int height)
    {
        glViewport(0, 0, width, height);

        // LOG_DEBUG("width: {0}, height: {1}", width, height);
    }

    void ControllerEvent::OnKey(int key, int action, int mods)
    {
        m_cameraController->OnKey(key, action, mods);

        // LOG_DEBUG("key: {0}, action: {1}, mods: {2}", key, action, mods);
    }

    void ControllerEvent::OnMouse(int button, int action, int mods)
    {
        double xpos, ypos;
        glfwGetCursorPos(m_window->GetWindowHandle(), &xpos, &ypos);
        m_cameraController->OnMouse(button, action, xpos, ypos);   

        // LOG_DEBUG("button: {0}, action: {1}, xpos: {2}, ypos: {3}", button, action, xpos, ypos);
    }

    void ControllerEvent::OnCursorPos(double xpos, double ypos)
    {
        m_cameraController->OnCursor(xpos, ypos);

        // LOG_DEBUG("xpos: {0}, ypos: {1}", xpos, ypos);
    }

    void ControllerEvent::OnScroll(double offset)
    {
        m_cameraController->OnScroll(offset);

        // LOG_DEBUG("offset: {0}", offset);
    }
}
