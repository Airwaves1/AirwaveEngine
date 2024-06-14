#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

class GLFWwindow;

namespace Airwave {


	//设置函数指针别名
	using m_ResizeCallback = void(*) (int width, int height);
	using m_KeyBoradCallback = void(*) (int key, int action, int mods);
	using m_MouseCallback = void(*) (int button, int action, int mods);
	using m_CursorPosCallback = void(*) (double xpos, double ypos);
	using m_ScrollCallback = void(*) (double offset);

	class EventManager
	{
	public:
		static EventManager* GetInstance();
		~EventManager() = default;

		void SetResizeCallback(m_ResizeCallback callback) { m_ResizeCallback = callback; }
		void SetKeyBoradCallback(m_KeyBoradCallback callback) { m_KeyBoradCallback = callback; }
		void SetMouseCallback(m_MouseCallback callback) { m_MouseCallback = callback; }
		void SetCursorPosCallback(m_CursorPosCallback callback) { m_CursorPosCallback = callback; }
		void SetScrollCallback(m_ScrollCallback callback) { m_ScrollCallback = callback; }

		

		//回调函数
		static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
		static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

		

	private:
		EventManager() = default;



	private:
		static EventManager* m_Instance;

		static GLFWwindow* m_Window;

		//函数指针
		static m_ResizeCallback m_ResizeCallback;
		static m_KeyBoradCallback m_KeyBoradCallback;
		static m_MouseCallback m_MouseCallback;
		static m_CursorPosCallback m_CursorPosCallback;
		static m_ScrollCallback m_ScrollCallback;


	};

}


#endif // !EVENT_MANAGER_H
