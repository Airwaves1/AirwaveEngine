#include "GLFWWindow.h"
#include "Utils/Log.h"

#ifdef GRAPHICS_API_OPENGL
	#include <glad/glad.h>
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

namespace Airwave {

	GLFWWindow::GLFWWindow(uint32_t width, uint32_t height, std::string title)
	{

		if (!glfwInit())
		{
			LOG_ERROR("Failed to initialize GLFW");
			return;
		}

#ifdef GRAPHICS_API_OPENGL

		#ifdef PLATFORM_MACOS
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#endif

		#if defined(PLATFORM_WIN32) || defined(PLATFORM_LINUX)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		#endif

#elif GRAPHICS_API_VULKAN
		GLFWWindowHint(GLFW_CLINET_API, GLFW_NO_API);
#endif

		m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!m_Window) {
			LOG_ERROR("Failed to create GLFW window");
			return;
		}

		//设置窗口居中
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		glfwSetWindowPos(m_Window, (mode->width - width) / 2, (mode->height - height) / 2);


		glfwMakeContextCurrent(m_Window);

	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	bool GLFWWindow::ShouldClose()
	{
		return glfwWindowShouldClose(m_Window);
	}

	void GLFWWindow::PollEvents()
	{
		glfwPollEvents();
	}

	void GLFWWindow::SwapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

	uint32_t GLFWWindow::GetWidth() {
		glfwGetWindowSize(m_Window, (int*)&m_Width, nullptr);
		return m_Width;
	}
	uint32_t GLFWWindow::GetHeight(){
		glfwGetWindowSize(m_Window, nullptr, (int*)&m_Height);
		return m_Height;
	}

}