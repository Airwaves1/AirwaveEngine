#include "EventManager.h"


#include <GLFW/glfw3.h>
#include "Utils/Log.h"

namespace Airwave {

	EventManager* EventManager::m_Instance = nullptr;
	GLFWwindow* EventManager::m_Window = nullptr;

	m_ResizeCallback EventManager::m_ResizeCallback = nullptr;
	m_KeyBoradCallback EventManager::m_KeyBoradCallback = nullptr;
	m_MouseCallback EventManager::m_MouseCallback = nullptr;
	m_CursorPosCallback EventManager::m_CursorPosCallback = nullptr;
	m_ScrollCallback EventManager::m_ScrollCallback = nullptr;


	EventManager* EventManager::GetInstance()
	{
		if (m_Instance == nullptr) {
			m_Instance = new EventManager();
		}

		return m_Instance;
	}



	void EventManager::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		if(m_Instance->m_ResizeCallback)
			m_Instance->m_ResizeCallback(width, height);
	}

	void EventManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if(m_Instance->m_KeyBoradCallback)
			m_Instance->m_KeyBoradCallback(key, action, mods);

	}

	void EventManager::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (m_Instance->m_MouseCallback)
			m_Instance->m_MouseCallback(button, action, mods);
		
	}

	void EventManager::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		if(m_Instance->m_CursorPosCallback)
			m_Instance->m_CursorPosCallback(xpos, ypos);
	}

	void EventManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		if(m_Instance->m_ScrollCallback)
			m_Instance->m_ScrollCallback(yoffset);
	}

}