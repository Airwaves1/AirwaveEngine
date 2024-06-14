#ifndef GLFW_WINDOW_H
#define GLFW_WINODW_H

#include "Utils/Common.h"
#include "Window.h"

class GLFWwindow;

namespace Airwave {


	class GLFWWindow : public Window {
	public:
		GLFWWindow() = delete;
		GLFWWindow(uint32_t width, uint32_t height, std::string title);
		~GLFWWindow() override;

		bool ShouldClose() override;
		void PollEvents() override;
		void SwapBuffers() override;

	private:

		GLFWwindow* m_Window;
	};

}



#endif // !GLFW_WINDOW_H
