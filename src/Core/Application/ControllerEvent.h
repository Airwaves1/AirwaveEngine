#ifndef CONTROLLER_EVENT_H
#define CONTROLLER_EVENT_H

#include "utils/Common.h"

namespace Airwave
{
    class CameraController;
    class Window;

    class ControllerEvent
    {
    public:

        static ControllerEvent* GetInstance();
        ControllerEvent() = default;
        ~ControllerEvent() = default;

        static void SetEvent(CameraController* cameraController, Window *window);

    private:
        static void OnResize(int width, int height);
        static void OnKey(int key, int action, int mods);
        static void OnMouse(int button, int action, int mods);
        static void OnCursorPos(double xpos, double ypos);
        static void OnScroll(double offset);

    private:


        static ControllerEvent *m_instance;
        static CameraController *m_cameraController;
        // static std::shared_ptr<CameraController> m_cameraController;
        static Window *m_window;

    };
}

#endif // !CONTROLLER_EVENT_H
