#include "CameraController.h"

#include <GLFW/glfw3.h>
#include "Camera/Camera.h"

namespace Airwave {
	CameraController::CameraController(Camera* camera)
		:m_Camera(camera)
	{
	}
	CameraController::~CameraController()
	{
	}
	void CameraController::Update(float deltaTime)
	{
	}
	void CameraController::OnMouse(int button, int action, double xpos, double ypos)
	{
		//1.判断是否按下
		bool pressed = (action == GLFW_PRESS);
		//2.记录按下的位置
		if (pressed)
		{
			m_CurrentX = xpos;
			m_CurrentY = ypos;
		}
		//3.根据不同的按键进行不同的操作
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			m_LeftMousePressed = pressed;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			m_RightMousePressed = pressed;
			break;
		case GLFW_MOUSE_BUTTON_MIDDLE:
			m_MiddleMousePressed = pressed;
			break;
		default:
			break;
		}
	}
	void CameraController::OnCursor(float xpos, float ypos)
	{
	}
	void CameraController::OnKey(int key, int action, int mods)
	{
		//过滤掉重复的按键
		if (action == GLFW_REPEAT)
		{
			return;
		}
		bool pressed = (action == GLFW_PRESS);

		m_KeyMap[key] = pressed;

	}
	void CameraController::OnScroll(double offset)
	{
	}
}